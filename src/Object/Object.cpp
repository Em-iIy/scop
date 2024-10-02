#include "Object.hpp"
#include "utils.hpp"

#include <cstring>
#include <iostream>
#include <unistd.h>
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

static e_obj_tok	check_token(std::string data)
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

void Object::fill_vertices()
{
	std::map<Vertex, GLuint> vertex_index_map;
	Vertex temp_vertex;

	this->indices.reserve(this->pos_indices.size());
	for (uint i = 0; i < this->pos_indices.size(); ++i)
	{
		GLuint index;
		if (this->multi_indexed == true)
		{
			temp_vertex = {
				.pos = this->pos[this->pos_indices[i]],
				.normal = this->normals[this->normal_indices[i]],
				.color = random_vec3(),
				.texUV = this->uvs[this->uv_indices[i]]
			};
		}
		else
		{
			mlm::vec3 pos = this->pos[this->pos_indices[i]];
			// temp solution for if the obj file doens't contain any normals or uv coordinates
			temp_vertex = {
				.pos = pos,
				.normal = pos,
				.color = random_vec3(),
				.texUV = mlm::vec2(pos.x, pos.y)
			};
		}
		std::cout << temp_vertex.color.x << " ";
		std::cout << temp_vertex.color.y << " ";
		std::cout << temp_vertex.color.z << std::endl;
		if (find_vertex_index(vertex_index_map, temp_vertex, index) == true)
		{
			this->indices.push_back(index);
		}
		else
		{
			this->vertices.push_back(temp_vertex);
			index = this->vertices.size() - 1;
			this->indices.push_back(index);
			vertex_index_map[temp_vertex] = index;
		}
	}
}

mlm::vec3	Object::get_center()
{
	mlm::vec3 ret(0.0f);
	for (std::vector<Vertex>::iterator it = this->vertices.begin(); it != this->vertices.end(); it++)
	{
		ret += it->pos;
	}
	ret /= this->vertices.size();
	for (std::vector<Vertex>::iterator it = this->vertices.begin(); it != this->vertices.end(); it++)
	{
		it->pos -= ret;
	}
	return (ret);
}

static void	parse_vector(std::vector<std::string> &params, std::vector<mlm::vec3> &vect)
{
	if (params.size() != 4)
	{
		throw std::runtime_error("Invalid vector");
	}
	vect.push_back(mlm::vec3(stof(params[1]), stof(params[2]), stof(params[3])));
}

static void	parse_uv(std::vector<std::string> &params, std::vector<mlm::vec2> &uvs)
{
	if (params.size() != 3)
	{
		throw std::runtime_error("Invalid texture uv");
	}
	uvs.push_back(mlm::vec2(stof(params[1]), stof(params[2])));
}

static void parse_triangle(std::string &idx1, std::string &idx2, std::string &idx3, std::vector<GLuint> &pos_indices, std::vector<GLuint> &normal_indices, std::vector<GLuint> &uv_indices)
{
	if (idx1.find('/') != std::string::npos)
	{
		std::vector<std::string> split_idx1 = split(idx1, "/");
		std::vector<std::string> split_idx2 = split(idx2, "/");
		std::vector<std::string> split_idx3 = split(idx3, "/");
		if (split_idx1.size() != 3 || split_idx2.size() != 3 || split_idx3.size() != 3)
		{
			throw std::runtime_error("Invalid face");
		}
		pos_indices.push_back(stoi(split_idx1[0]) - 1);
		pos_indices.push_back(stoi(split_idx2[0]) - 1);
		pos_indices.push_back(stoi(split_idx3[0]) - 1);
		uv_indices.push_back(stoi(split_idx1[1]) - 1);
		uv_indices.push_back(stoi(split_idx2[1]) - 1);
		uv_indices.push_back(stoi(split_idx3[1]) - 1);
		normal_indices.push_back(stoi(split_idx1[2]) - 1);
		normal_indices.push_back(stoi(split_idx2[2]) - 1);
		normal_indices.push_back(stoi(split_idx3[2]) - 1);
	}
	else
	{
		pos_indices.push_back(stoi(idx1) - 1);
		pos_indices.push_back(stoi(idx2) - 1);
		pos_indices.push_back(stoi(idx3) - 1);
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
	std::vector<std::string> params = split(line, " ");

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
}


Object::Object(const std::string &file_path, const std::string &file_name)
{
	std::cout << "loading " << file_path + file_name << "..." << std::endl;
	char *data = read_file((file_path + file_name).c_str());
	size_t i = 0;
	if (!data)
	{
		std::cerr << "Object: Could not open " << file_path + file_name << std::endl;
		throw std::exception();
	}
	std::vector<std::string> lines = split(data, "\n");
	try
	{
		for (i = 0; i < lines.size(); i++)
		{
			parse_line(lines[i]);
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
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << file_path + file_name << ":" << i << " " << e.what() << '\n';
		free(data);
		throw std::exception();
	}
	fill_vertices();
	free(data);
	this->pos.clear();
	this->normals.clear();
	this->uvs.clear();
}

Object::~Object()
{
}

const std::vector<Vertex>	&Object::get_vertices()
{
	return (this->vertices);
}

const std::vector<GLuint>	&Object::get_indices()
{
	return (this->indices);
}

