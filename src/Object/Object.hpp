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

	std::vector<mlm::vec3>	coords;
	std::vector<mlm::vec3>	normals;
	std::vector<mlm::vec2>	texUVs;

	void	parse_coords(char *data, int i, int nl);
	void	parse_tex(char *data, int i, int nl);
	void	parse_face(char *data, int i, int nl);

	void	fill_vertices();
public:
	Object(const std::string &file_path, const std::string &file_name);
	~Object();

	const std::vector<Vertex>	&get_vertices();
	const std::vector<GLuint>	&get_indices();
};
