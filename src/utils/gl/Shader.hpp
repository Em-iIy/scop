/*
Created by: Emily (Em_iIy) Winnink
Created on: 07/09/2024
*/
#pragma once

#include <string>

class Shader {
	public:
		unsigned int _id;
	

		Shader(const char *vertexFileName, const char *fragmentFileName);
		~Shader();
		void use();
		void Delete();
		void setBool(const std::string &name, bool value) const;
		void setFloat(const std::string &name, float value) const;
		void setInt(const std::string &name, int value) const;
		void setMat4(const std::string &name, mlm::mat4 &value) const;

};