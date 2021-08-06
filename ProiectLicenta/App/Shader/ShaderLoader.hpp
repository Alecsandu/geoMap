#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "glm.hpp"

class ShaderLoader
{
public:
	ShaderLoader();
	ShaderLoader(const std::string&, const std::string&);
	~ShaderLoader();

	void cleanup();

	void Bind() const;
	void Unbind() const;

	void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
	void set_uniform_matrix_4fv(const std::string& name, glm::mat4);
	void set_uniform_1i(const std::string& name, int);

	unsigned int getProgramId();

private:
	std::string	 read_file(const std::string&);
	unsigned int compile_shader(unsigned int, const std::string&);
	void		 create_shaders(const std::string&, const std::string&);
	unsigned int get_uniform_location(const std::string&);

	unsigned int m_program_id;
	bool m_flag_shader;
};