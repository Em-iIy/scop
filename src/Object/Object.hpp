#pragma once

#include <vector>
#include <string>
#include <GLAD/glad.h>
#include <emlm/emlm.hpp>
#include "utils.hpp"

enum e_obj_tok {
	SKIP,
	VERTEX,
	NORMAL,
	TEX,
	FACE,
};

class Object
{
private:
	std::vector<Vertex>		vertices;
	std::vector<GLuint>		indices;

	std::vector<GLuint>		pos_indices;
	std::vector<GLuint>		normal_indices;
	std::vector<GLuint>		uv_indices;

	std::vector<mlm::vec3>	pos;
	std::vector<mlm::vec3>	normals;
	std::vector<mlm::vec2>	uvs;

	bool					multi_indexed = false;

	void	parse_line(std::string &line);

	void	fill_vertices();
public:
	Object(const std::string &file_path, const std::string &file_name);
	~Object();

	mlm::vec3						get_center();

	const std::vector<Vertex>		&get_vertices();
	const std::vector<GLuint>		&get_indices();
};
