#pragma once

#include <vector>
#include <string>
#include <GLAD/glad.h>
#include <emlm/emlm.hpp>
#include "utils.hpp"

#define MOVE_SPEED 10.0f
#define SCALE_SPEED 5.0f

enum e_obj_tok {
	SKIP,
	VERTEX,
	NORMAL,
	TEX,
	FACE,
};

enum e_object_movement {
	OBJ_FORWARD,
	OBJ_BACKWARD,
	OBJ_LEFT,
	OBJ_RIGHT,
	OBJ_UP,
	OBJ_DOWN,
	OBJ_SCALE_UP,
	OBJ_SCALE_DOWN
};

class Object
{
private:
	std::vector<Vertex>		vertices;
	std::vector<GLuint>		indices;

	VAO						vao;
	VBO						vbo;
	EBO						ebo;

	std::vector<GLuint>		pos_indices;
	std::vector<GLuint>		normal_indices;
	std::vector<GLuint>		uv_indices;

	std::vector<mlm::vec3>	pos;
	std::vector<mlm::vec3>	normals;
	std::vector<mlm::vec2>	uvs;

	mlm::vec3				center;
	mlm::vec3				position;
	mlm::vec3				scale;

	bool					multi_indexed;

	void	parse_line(std::string &line);

	void	fill_vertices();

	void	center_vertices();

	void	clean_temp();

	void	init_buffers();
public:
	Object();
	Object(const std::string &file_name);
	~Object();

	void							load_file(const std::string &file_name);
	void							load(const char *data);

	void							bind();
	void							unbind();
	void							draw();

	void							del();
	
	void 							process_keyboard(e_object_movement direction, float &delta_time);

	const mlm::vec3					&get_center() const;
	const mlm::vec3					&get_position() const;
	void							set_position(const mlm::vec3 &v);
	const mlm::vec3					&get_scale() const;
	void							set_scale(const mlm::vec3 &v);

	const std::vector<Vertex>		&get_vertices() const;
	const std::vector<GLuint>		&get_indices() const;
};
