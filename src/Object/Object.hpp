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

	mlm::vec3				center;
	mlm::vec3				position;

	bool					multi_indexed;

	void	parse_line(std::string &line);

	void	fill_vertices();

	void	center_vertices();

	void	clean_temp();
public:
	Object();
	Object(const std::string &file_name);
	~Object();

	void							load(const std::string &file_name);

	const mlm::vec3					&get_center() const;
	const mlm::vec3					&get_position() const;
	void							set_position(const mlm::vec3 &v);

	const std::vector<Vertex>		&get_vertices() const;
	const std::vector<GLuint>		&get_indices() const;
};
