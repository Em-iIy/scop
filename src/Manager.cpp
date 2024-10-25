#include "Manager.hpp"

std::map<std::string, Tex2d>	Manager::textures;
std::map<std::string, Shader>	Manager::shaders;
std::map<std::string, Object>	Manager::objects;

Manager::Manager() {}

void	Manager::load_texture_file(Tex2d &tex, const char *file_name)
{
	bmp_t	bmp = load_bmp(file_name);
	if (!bmp.data)
	{
		std::cout << "load_texture: could not load: " << file_name << std::endl;
		throw std::exception();
	}
	tex.load(bmp);
	free_bmp(bmp);
}

void	Manager::load_shader_file(Shader &shader, const char *vertex_file_name, const char *fragment_file_name)
{
	char *vertex_source = read_file(vertex_file_name);
	char *fragment_source = read_file(fragment_file_name);
	if (!vertex_source || !fragment_source)
	{
		std::cout << "load_shader: could not load shaders" << std::endl;
		free(vertex_source);
		free(fragment_source);
		throw std::exception();
	}
	try
	{
		shader.load(vertex_source, fragment_source);
	}
	catch(...)
	{
		free(vertex_source);
		free(fragment_source);
		throw std::exception();
	}
	
	free(vertex_source);
	free(fragment_source);
}

void	Manager::load_object_file(Object &obj, const char *file_name)
{
	std::cout << "loading " << file_name << "..." << std::endl;
	char *data = read_file(file_name);
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
		std::cerr << file_name << ": " << e.what() << '\n';
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

Tex2d	&Manager::get_texture(const std::string &name)
{
	return (Manager::textures[name]);
}

Shader	&Manager::get_shader(const std::string &name)
{
	return (Manager::shaders[name]);
}

Object	&Manager::get_object(const std::string &name)
{
	return (Manager::objects[name]);
}

void	Manager::clear(void)
{
	for (std::pair<const std::string, Tex2d> &tex : Manager::textures)
	{
		tex.second.del();
	}
	for (std::pair<const std::string, Shader> &shader : Manager::shaders)
	{
		shader.second.del();
	}
}
