#include "Object.hpp"
#include "utils.hpp"

#include <cstring>
#include <iostream>
#include <map>

static mlm::vec3	random_vec3(void)
{
	mlm::vec3 ret;
	ret.x = (float)rand();
	ret.y = (float)rand();
	ret.z = (float)rand();
	ret /= 2147483648;
	return (ret);
}

static e_obj_tok	check_token(std::string &data)
{
	switch (data[0])
	{
	case 'v':
		switch (data[1])
		{
		case 'n':
			return (NORMAL);
			break;
		case 't':
			return (TEX);
			break;
		default:
			return (VERTEX);
			break;
		}
		break;
	case 'f':
		return (FACE);
		break;
	case '#':
		return (SKIP);
		break;
	case '\n':
		return (SKIP);
		break;
	case 'o':
		return (SKIP); // implement later
		break;
	case 's':
		return (SKIP);
		break;
	default:
		if (data == "mtllib")
			return (SKIP); // implement later
		if (data == "usemtl")
			return (SKIP);
		break;
	}
	return (SKIP);
}

// Searches the vertex_index_map to see if a vertex already exists
// If it does, the it can reuse the index, rather than create a new vertex
static bool	find_vertex_index(std::map<Vertex, GLuint> &vertex_index_map, Vertex &vertex, GLuint &index)
{
	std::map<Vertex, GLuint>::iterator it = vertex_index_map.find(vertex);
	if (it == vertex_index_map.end())
	{
		return (false);
	}
	// Assign the index associated with the vertex
	index = it->second;
	return (true);
}

// Fills the vertices vector with the data gathered while parsing in the Vertex format
// It does so by reindexing to go from 3 separate vectors of indices to 1
void Object::fill_vertices()
{
	std::map<Vertex, GLuint> vertex_index_map;
	
	Vertex temp_vertex;
	this->indices.reserve(this->pos_indices.size());
	for (uint i = 0; i < this->pos_indices.size(); ++i)
	{
		GLuint index;
		if (this->pos_indices[i] >= this->pos.size())
		{
			throw std::runtime_error("Index out of range");
		}
		if (this->multi_indexed == true)
		{
			if (this->normal_indices[i] >= this->normals.size() || this->uv_indices[i] >= this->uvs.size())
			{
				throw std::runtime_error("Index out of range");
			}
			temp_vertex = {
				.pos = this->pos[this->pos_indices[i]],
				.normal = this->normals[this->normal_indices[i]],
				.color = mlm::vec3(1.0f),
				// .color = random_vec3(),
				.texUV = this->uvs[this->uv_indices[i]]
			};
		}
		else
		{
			mlm::vec3 pos = this->pos[this->pos_indices[i]];
			// solution for if the obj file doens't contain any normals or uv coordinates
			temp_vertex = {
				.pos = pos,
				.normal = pos,
				// .color = mlm::vec3(1.0f),
				.color = random_vec3(),
				.texUV = mlm::vec2(pos.z, pos.y)
			};
		}
		if (find_vertex_index(vertex_index_map, temp_vertex, index) == true)
		{
			this->indices.emplace_back(index);
		}
		else
		{
			this->vertices.emplace_back(temp_vertex);
			index = this->vertices.size() - 1;
			this->indices.emplace_back(index);
			vertex_index_map[temp_vertex] = index;
		}
	}
}

// Finds the center by averaging the positions of the vertices
// Then moves all vertices by that average, centering them all around 0,0,0
void	Object::center_vertices()
{
	this->center = mlm::vec3(0.0f);
	for (mlm::vec3 &v : this->pos)
	{
		this->center += v;
	}
	this->center /= this->pos.size();
	for (mlm::vec3 &v : this->pos)
	{
		v -= this->center;
	}
}

static void	parse_vector(std::vector<std::string> &params, std::vector<mlm::vec3> &vect)
{
	if (params.size() != 4)
	{
		throw std::runtime_error("Invalid vector");
	}
	vect.emplace_back(mlm::vec3(stof(params[1]), stof(params[2]), stof(params[3])));
}

static void	parse_uv(std::vector<std::string> &params, std::vector<mlm::vec2> &uvs)
{
	if (params.size() != 3)
	{
		throw std::runtime_error("Invalid texture uv");
	}
	uvs.emplace_back(mlm::vec2(stof(params[1]), stof(params[2])));
}

static void parse_triangle(std::string &idx1, std::string &idx2, std::string &idx3, std::vector<GLuint> &pos_indices, std::vector<GLuint> &normal_indices, std::vector<GLuint> &uv_indices)
{
	static std::vector<std::string> split_idx1;
	static std::vector<std::string> split_idx2;
	static std::vector<std::string> split_idx3;
	split_idx1.reserve(3);
	split_idx2.reserve(3);
	split_idx3.reserve(3);

	if (idx1.find('/') != std::string::npos)
	{
		split(split_idx1, idx1, "/");
		split(split_idx2, idx2, "/");
		split(split_idx3, idx3, "/");
		if (split_idx1.size() != 3 || split_idx2.size() != 3 || split_idx3.size() != 3)
		{
			throw std::runtime_error("Invalid face");
		}
		pos_indices.emplace_back(stoi(split_idx1[0]) - 1);
		pos_indices.emplace_back(stoi(split_idx2[0]) - 1);
		pos_indices.emplace_back(stoi(split_idx3[0]) - 1);
		uv_indices.emplace_back(stoi(split_idx1[1]) - 1);
		uv_indices.emplace_back(stoi(split_idx2[1]) - 1);
		uv_indices.emplace_back(stoi(split_idx3[1]) - 1);
		normal_indices.emplace_back(stoi(split_idx1[2]) - 1);
		normal_indices.emplace_back(stoi(split_idx2[2]) - 1);
		normal_indices.emplace_back(stoi(split_idx3[2]) - 1);
		split_idx1.clear();
		split_idx2.clear();
		split_idx3.clear();
	}
	else
	{
		pos_indices.emplace_back(stoi(idx1) - 1);
		pos_indices.emplace_back(stoi(idx2) - 1);
		pos_indices.emplace_back(stoi(idx3) - 1);
	}
}

static void parse_face(std::vector<std::string> &params, std::vector<GLuint> &pos_indices, std::vector<GLuint> &normal_indices, std::vector<GLuint> &uv_indices)
{
	if (params.size() < 4)
	{
		throw std::runtime_error("Invalid face");
	}
	for (size_t i = 2; i + 1 < params.size(); ++i)
	{
		parse_triangle(params[1], params[i], params[i + 1], pos_indices, normal_indices, uv_indices);
	}
}

void	Object::parse_line(std::string &line)
{
	static std::vector<std::string> params;
	params.reserve(16);
	split(params, line, " ");
	switch (check_token(params[0]))
	{
	case SKIP:
		break;
	case VERTEX:
		parse_vector(params, this->pos);
		break;
	case NORMAL:
		parse_vector(params, this->normals);
		break;
	case TEX:
		parse_uv(params, this->uvs);
		break;
	case FACE:
		parse_face(params, this->pos_indices, this->normal_indices, this->uv_indices);
		break;

	default:
		break;
	}
	params.clear();
}

// Resizes all temporary vectors to have a capacity of 0
// This memory would get cleaned up by garbage collection, however this would only happen at the end.
void	Object::clean_temp()
{
	// uint64_t bytes_cleaned = this->pos.size() + this->normals.size() + this->uvs.size() + this->pos_indices.size() + this->normal_indices.size() + this->uv_indices.size();
	// std::cout << "Bytes freed: " << bytes_cleaned << std::endl;
	this->pos.clear();
	this->pos.shrink_to_fit();
	this->normals.clear();
	this->normals.shrink_to_fit();
	this->uvs.clear();
	this->uvs.shrink_to_fit();
	this->pos_indices.clear();
	this->pos_indices.shrink_to_fit();
	this->normal_indices.clear();
	this->normal_indices.shrink_to_fit();
	this->uv_indices.clear();
	this->uv_indices.shrink_to_fit();
}

void	Object::init_buffers()
{
	// Create a Vertex Array Object to keep track of the location and layout of the vertex buffer
	this->vao = VAO(1);
	this->vao.bind();

	// Create buffers objects for the vertices and indices and store them on GPU
	this->vbo = VBO((GLfloat *)vertices.data(), vertices.size() * sizeof(Vertex));
	this->ebo = EBO((GLuint *)indices.data(), indices.size() * sizeof(GLuint));

	this->vao.link_attr(this->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
	this->vao.link_attr(this->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(GLfloat)));
	this->vao.link_attr(this->vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(GLfloat)));
	this->vao.link_attr(this->vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(GLfloat)));
	this->vao.unbind();
}

Object::Object(): position(mlm::vec3(0.0f)), multi_indexed(false)
{
}

Object::Object(const std::string &file_name): position(mlm::vec3(0.0f)), multi_indexed(false)
{
	this->load_file(file_name);
}

Object::~Object()
{
}

void	Object::load_file(const std::string &file_name)
{
	std::cout << "loading " << file_name << "..." << std::endl;
	char *data = read_file((file_name).c_str());
	size_t i = 0;
	if (!data)
	{
		std::cerr << "Object: Could not open " << file_name << std::endl;
		throw std::exception();
	}
	try
	{
		this->load(data);
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << file_name << ":" << i << " " << e.what() << '\n';
		free(data);
		throw std::exception();
	}
	free(data);
}

void	Object::load(const char *data)
{
	size_t i = 0;
	std::vector<std::string> lines = split(data, "\n");
	for (i = 0; i < lines.size(); i++)
	{
		this->parse_line(lines[i]);
	}
	if (this->uv_indices.size() == 0 && this->normal_indices.size() == 0)
	{
		this->multi_indexed = false;
	}
	else if (this->uv_indices.size() == this->pos_indices.size() || this->normal_indices.size() == this->pos_indices.size())
	{
		this->multi_indexed = true;
	}
	else
	{
		throw std::runtime_error("Invalid face indexing");
	}
	this->center_vertices();
	this->fill_vertices();
	this->init_buffers();
	this->clean_temp();
}

void	Object::bind()
{
	this->vao.bind();
}

void	Object::unbind()
{
	this->vao.unbind();
}

void	Object::draw()
{
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

void	Object::del()
{
	this->ebo.del();
	this->vbo.del();
	this->vao.del();
}


void Object::process_keyboard(e_object_movement direction, float &delta_time)
{
	float velocity = MOVE_SPEED * delta_time;

	switch (direction)
	{
	case OBJ_FORWARD:
		this->position += mlm::vec3(0.0f, 0.0f, -1.0f) * velocity;
		break ;
	case OBJ_BACKWARD:
		this->position -= mlm::vec3(0.0f, 0.0f, -1.0f) * velocity;
		break ;
	case OBJ_LEFT:
		this->position += mlm::vec3(-1.0f, 0.0f, 0.0f) * velocity;
		break ;
	case OBJ_RIGHT:
		this->position -= mlm::vec3(-1.0f, 0.0f, 0.0f) * velocity;
		break ;
	case OBJ_UP:
		this->position += mlm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		break ;
	case OBJ_DOWN:
		this->position -= mlm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		break ;
	case OBJ_SCALE_UP:
		this->scale += SCALE_SPEED * delta_time;
		break ;
	case OBJ_SCALE_DOWN:
		this->scale -= SCALE_SPEED * delta_time;
		break ;
	default:
		break;
	}
}

const std::vector<Vertex>	&Object::get_vertices() const
{
	return (this->vertices);
}

const std::vector<GLuint>	&Object::get_indices() const
{
	return (this->indices);
}

const mlm::vec3	&Object::get_center() const
{
	return (this->center);
}

const mlm::vec3	&Object::get_position() const
{
	return (this->position);
}

void	Object::set_position(const mlm::vec3 &v)
{
	this->position = v;
}

const mlm::vec3	&Object::get_scale() const
{
	return (this->scale);
}

void	Object::set_scale(const mlm::vec3 &v)
{
	this->scale = v;
}
