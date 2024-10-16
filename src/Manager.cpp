#include "Manager.hpp"

Manager::Manager() {}

void	Manager::load_texture_file(Tex2d &tex, const char *file_name)
{
	bmp_t	bmp = load_bmp(file_name);
	if (!bmp.data)
	{
		std::cout << "load_texture: could not load: " << file_name << std::endl;
		exit(EXIT_FAILURE);
	}
	tex.load(bmp);
	free_bmp(bmp);
}

void	Manager::load_shader_file(Shader &shader, const char *vertex_file_name, const char *fragment_file_name)
{
	char *vertex_source = read_file(vertex_file_name);
	if (!vertex_source)
		return ;
	char *fragment_source = read_file(fragment_file_name);
	if (!fragment_source)
	{
		free(vertex_source);
		return ;
	}
	shader.load(vertex_source, fragment_source);
	free(vertex_source);
	free(fragment_source);
}

void	Manager::load_object_file(Object &obj, const char *file_name)
{
	std::cout << "loading " << file_name << "..." << std::endl;
	char *data = read_file(file_name);
	size_t i = 0;
	if (!data)
	{
		std::cerr << "Object: Could not open " << file_name << std::endl;
		throw std::exception();
	}
	try
	{
		obj.load(data);
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << file_name << ":" << i << " " << e.what() << '\n';
		free(data);
		throw std::exception();
	}
	free(data);
}


void	Manager::load_texture(const std::string &name, const char *file_name)
{
	Manager::textures.emplace(name, Tex2d());
	Manager::load_texture_file(Manager::textures[name], file_name);
}

void	Manager::load_shader(const std::string &name, const char *vertex_file_name, const char *fragment_file_name)
{
	Manager::shaders.emplace(name, Shader());
	Manager::load_shader_file(Manager::shaders[name], vertex_file_name, fragment_file_name);
}

void	Manager::load_object(const std::string &name, const char *file_name)
{
	Manager::objects.emplace(name, Object());
	Manager::load_object_file(Manager::objects[name], file_name);
}

