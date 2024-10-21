#include <map>
#include <string>

#include "Object/Object.hpp"
#include "utils/utils.hpp"

class Manager {
	private:
		Manager();

		static void		load_texture_file(Tex2d &tex, const char *file_name);
		static void		load_shader_file(Shader &shader, const char *vertex_file_name, const char *fragment_file_name);
		static void		load_object_file(Object &obj, const char *file_name);


	public:
		static std::map<std::string, Tex2d>		textures;
		static std::map<std::string, Shader>	shaders;
		static std::map<std::string, Object>	objects;

		static void		load_texture(const std::string &name, const char *file_name);
		static void		load_shader(const std::string &name, const char *vertex_file_name, const char *fragment_file_name);
		static void		load_object(const std::string &name, const char *file_name);

		static Tex2d	&get_texture(const std::string &name);
		static Shader	&get_shader(const std::string &name);
		static Object	&get_object(const std::string &name);

		static void		clear(void);
};