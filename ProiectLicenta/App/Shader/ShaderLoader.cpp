#include "ShaderLoader.hpp"

ShaderLoader::ShaderLoader() 
{
	m_flag_shader = false;
	m_program_id = 0;
}

ShaderLoader::ShaderLoader(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
	m_flag_shader = true;
	std::string vertex_shader_code = read_file(vertex_file_path);
	std::string fragment_shader_code = read_file(fragment_file_path);
	create_shaders(vertex_shader_code, fragment_shader_code);
}

ShaderLoader::~ShaderLoader()
{
}

void ShaderLoader::cleanup()
{
	m_flag_shader = false;
	if (m_program_id != 0)
		glDeleteProgram(m_program_id);
}

void ShaderLoader::Bind() const
{
	glUseProgram(m_program_id);
}

void ShaderLoader::Unbind() const
{
	glUseProgram(0);
}

void ShaderLoader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void ShaderLoader::set_uniform_matrix_4fv(const std::string& name, glm::mat4 matr)
{
	glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matr[0][0]);
}

void ShaderLoader::set_uniform_1i(const std::string& name, int v)
{
	glUniform1i(get_uniform_location(name), v);
}

unsigned int ShaderLoader::getProgramId()
{
	return this->m_program_id;
}

std::string ShaderLoader::read_file(const std::string& file_path)
{
	std::ifstream f_in(file_path, std::ios::in);

	if (!f_in.is_open())
	{
		std::cerr << "Nu se poate citii din " << file_path << ". Fisierul nu exista." << std::endl;
	}
	std::stringstream buffer;
	std::string line;
	while (std::getline(f_in, line))
	{
		buffer << line << "\n";
	}
	f_in.close();
	return buffer.str();
}

unsigned int ShaderLoader::compile_shader(unsigned int type, const std::string& source)
{
	// creez un obiect shader gol, care va primii cod si va fi compilat
	unsigned int compiled_shader = glCreateShader(type);
	
	const char* src = source.c_str();
	glShaderSource(compiled_shader, 1, &src, nullptr);

	glCompileShader(compiled_shader);

	/* verificare daca shaderul a fost creat cu succes */
	int result;
	glGetShaderiv(compiled_shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(compiled_shader, GL_INFO_LOG_LENGTH, &length);
		char* info_log = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(compiled_shader, length, &length, info_log);
		std::cout << "Eroare la compilarea shaderului de " << (type == GL_VERTEX_SHADER ? "varfuri" : "fragment") << " !" << std::endl;
		std::cout << info_log << std::endl;

		glDeleteShader(compiled_shader);

		return static_cast<unsigned int>(0);
	}

	return compiled_shader;
}

void ShaderLoader::create_shaders(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
	if (vs == 0 || fs == 0)
		return;

	m_program_id = glCreateProgram();

	glAttachShader(m_program_id, vs);
	glAttachShader(m_program_id, fs);

	glLinkProgram(m_program_id);

	int result;
	glGetShaderiv(m_program_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
		char* info_log = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(m_program_id, length, &length, info_log);
		std::cout << "Eroare la legarea shaderelor!"
			<< std::endl;
		std::cout << info_log << std::endl;
		glDeleteProgram(m_program_id);
		return;
	}
	glValidateProgram(m_program_id);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

unsigned int ShaderLoader::get_uniform_location(const std::string& name)
{
	unsigned int location = glGetUniformLocation(m_program_id, name.c_str());
	if (location == -1)
		std::cout << "Atentie uniforma '" << name << "' nu exisa!" << std::endl;
	return location;
}
