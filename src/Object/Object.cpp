#include "Object.hpp"
#include "utils.hpp"

#include <cstring>
#include <iostream>
#include <unistd.h>

static e_obj_tok	check_token(char *data, int i)
{
	if (data[i] == 'v')
	{
		if (data[i + 1] == ' ')
			return (VERTEX);
		if (data[i + 1] == 'n')
			return (NORMAL);
		if (data[i + 1] == 't')
			return (TEX);
	}
	if (data[i] == 'f')
		return (FACE);
	if (data[i] == '#')
		return (SKIP);
	if (data[i] == '\n')
		return (SKIP);
	if (data[i] == '0')
		return (SKIP);
	return (SKIP);
}

void	Object::parse_coords(char *data, int i, int nl)
{
	mlm::vec3 ret;
	char *temp = strchr(&data[i], ' ');
	int j = 0;
	while (temp && temp < &data[i + nl])
	{
		if (j == 3)
			throw std::runtime_error("Too many coordinates");
		ret[j] = atof(temp);
		j++;
		temp = strchr(++temp, ' ');
	}
	if (j < 3)
		throw std::runtime_error("Too little coordinates");
	this->coords.push_back(ret);
}

void	Object::parse_tex(char *data, int i, int nl)
{
	mlm::vec2 ret;
	char *temp = strchr(&data[i], ' ');
	int j = 0;
	while (temp && temp < &data[i + nl])
	{
		if (j == 2)
			throw std::runtime_error("Too many coordinates");
		ret[j] = atof(temp);
		j++;
		temp = strchr(++temp, ' ');
	}
	if (j < 2)
		throw std::runtime_error("Too little coordinates");
	this->texUVs.push_back(ret);
}

void	Object::parse_face(char *data, int i, int nl)
{
	GLuint	temp_indices[32];
	char *temp = strchr(&data[i], ' ');
	int j = 0;
	int k = 1;
	while (temp && temp < &data[i + nl])
	{
		temp_indices[j] = atoi(temp) - 1;
		j++;
		temp = strchr(temp + 1, ' ');
	}
	if (j < 3)
		throw std::runtime_error("Too little indices");
	while (k + 1 < j)
	{
		this->indices.push_back(temp_indices[0]);
		this->indices.push_back(temp_indices[k]);
		this->indices.push_back(temp_indices[k + 1]);
		k++;
	}
}


void	Object::fill_vertices()
{
	vertices.reserve(this->coords.size());
	for (size_t i = 0; i < this->coords.size(); i++)
	{
		mlm::vec2 tex;
		if (i < this->texUVs.size())
			tex = this->texUVs[i];
		else
			tex = mlm::vec2(this->coords[i].x, this->coords[i].y);

		this->vertices.push_back(Vertex{
			this->coords[i],
			mlm::vec3(1.0f),
			tex
		});
	}
}


Object::Object(const std::string &file_path, const std::string &file_name)
{
	std::cout << "loading " << file_path + file_name << "..." << std::endl;
	char *data = read_file((file_path + file_name).c_str());
	if (!data)
	{
		std::cerr << "Object: Could not open " << file_path + file_name << std::endl;
		throw std::exception();
	}
	int line_count = 0;
	int i = 0;
	int nl = 0;
	bool last_line = false;
	try
	{
		while (last_line != true && data[i])
		{
			line_count++;
			char *temp = strchr(&data[i], '\n');
			if (!temp)
			{
				last_line = true;
				nl = strlen(&data[i]);
			}
			else
			{
				nl = temp - &data[i];
			}
			switch (check_token(data, i))
			{
			case SKIP:
				break;
			case VERTEX:
				this->parse_coords(data, i, nl);
				break;
			case NORMAL:
				break;
			case TEX:
				this->parse_tex(data, i, nl);
				break;
			case FACE:
				this->parse_face(data, i, nl);
				break;
			
			default:
				break;
			}
			i += nl + 1;
		}
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << file_path + file_name << ":" << line_count << " " << e.what() << '\n';
		free(data);
		throw std::exception();
	}
	fill_vertices();
	free(data);
	this->coords.clear();
	this->normals.clear();
	this->texUVs.clear();
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

