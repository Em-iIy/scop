#include <cmath>

#include "Scop.hpp"
#include "utils/utils.hpp"
#include "Manager.hpp"

extern float	g_delta_time;

Scop::Scop()
{
	this->state = {
		.width = 0,
		.height = 0,
		.tex_mode = 0,
		.wf_mode = 0,
		.angle = 0.0f,
		.tex_mix = 0.0f,
		.rotate_speed = 45.0f
	};
	this->initted = {
		.gl = false,
		.window = false,
		.resources = false
	};
}

void	Scop::del()
{
	if (this->initted.resources == true)
	{
		this->ebo.del();
		this->vbo.del();
		this->vao.del();
	}
	if (this->initted.window == true)
		glfwDestroyWindow(this->window);
	if (this->initted.gl == true)
		glfwTerminate();
}

void	Scop::init_gl(const int w, const int h)
{
	// Set the random seed
	srand(time(NULL));
	// Initialize glfw and set opengl version
	init_glfw();
	this->initted.gl = true;
	// Create window
	this->state.width = w;
	this->state.height = h;
	this->window = init_window(this->state.width, this->state.height, "scop", NULL, NULL);
	this->initted.window = true;
	// Set callback functions
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(this->window, mouse_callback);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

	// Set opengl render settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void	Scop::init_resources(const char *object, const char *texture)
{
	// Read and compile the shaders
	this->current_shader = "default";
	Manager::load_shader("default", "./resources/shaders/default.vert", "./resources/shaders/default.frag");
	Shader	&shader = Manager::get_shader(this->current_shader);
	// Set the uniform for tex0
	shader.use();
	shader.set_int("tex0", 0);

	// Read and load the texture
	this->current_texture = "input";
	Manager::load_texture("input", texture);

	// Load in the input object
	this->current_object = "input";
	Manager::load_object("input", object);
	Object	&obj = Manager::get_object(this->current_object);
	// Move and scale object in front of the camera
	obj.set_position(mlm::vec3(0.0f, 0.0f, -10.0f));
	obj.set_scale(mlm::vec3(1.0f));

	// Create a Vertex Array Object to keep track of the location and layout of the vertex buffer
	this->vao = VAO(1);
	this->vao.bind();

	// Create buffers objects for the vertices and indices and store them on GPU
	const std::vector<Vertex> &vertices = obj.get_vertices();
	const std::vector<GLuint> &indices = obj.get_indices();
	this->vbo = VBO((GLfloat *)vertices.data(), vertices.size() * sizeof(Vertex));
	this->ebo = EBO((GLuint *)indices.data(), indices.size() * sizeof(GLuint));

	// Specify the data layout of the vertices
	this->vao.link_attr(this->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
	this->vao.link_attr(this->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(GLfloat)));
	this->vao.link_attr(this->vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(GLfloat)));
	this->vao.link_attr(this->vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(GLfloat)));
	this->vao.unbind();
	this->initted.resources = true;
}

void	Scop::update()
{
	// Check for window input (mouse & keyboard)
	glfwPollEvents();
	process_input(this->window);

	// Update delta_time with the time last frame took to render
	g_delta_time = delta_time_update();

	// Handle smooth transition from color to texture and back
	if (this->state.tex_mode % 2 == 1 && this->state.tex_mix < 1.0f)
	{
		this->state.tex_mix += 1.0f * g_delta_time;
	}
	else if (this->state.tex_mode % 2 == 0 && this->state.tex_mix > 0.0f)
	{
		this->state.tex_mix -= 1.0f * g_delta_time;
	}
	this->state.tex_mix = fminf32(fmaxf32(this->state.tex_mix, 0.0f), 1.0f);

	// Update how much the object has rotated
	this->state.angle += this->state.rotate_speed * g_delta_time;
}

void	Scop::draw_current()
{
	// Refresh the background to black and clear the framebuffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Fetch object, shader and texture from the manager
	Object	&object = Manager::get_object(this->current_object);
	Shader	&shader = Manager::get_shader(this->current_shader);
	Tex2d	&texture = Manager::get_texture(this->current_texture);

	// Get the camera view matrix
	mlm::mat4 view = this->camera.get_matrix();

	// Get the perspective matrix
	mlm::mat4 projection = mlm::perspective(mlm::radians(this->camera.get_fov()), (float)this->state.width / (float)this->state.height, 0.1f, 100.0f);

	// Translate, scale and rotate the model matrix to the current values
	mlm::mat4 model(1.0f);
	model = mlm::translate(model, object.get_position());
	model = mlm::scale(model, object.get_scale());
	model = mlm::rotate(model, mlm::radians(90.0f), mlm::vec3(0.0f, 1.0f, 0.0f));
	model = mlm::rotate(model, mlm::radians(this->state.angle), mlm::vec3(0.0f, 1.0f, 0.0f));

	shader.use();
	// Set the uniforms of the texture and matrices
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	shader.set_float("tex_mix", this->state.tex_mix);
	shader.set_mat4("model", model);
	shader.set_mat4("projection", projection);
	shader.set_mat4("view", view);

	// Draw the object to the framebuffer
	this->vao.bind();
	glDrawElements(GL_TRIANGLES, object.get_indices().size(), GL_UNSIGNED_INT, 0);
	// Swap the drawn buffer to the one in the window
	glfwSwapBuffers(this->window);
}
