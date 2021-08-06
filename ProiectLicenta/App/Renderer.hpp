#pragma once
#include <GL/glew.h>
#include "VertexArray.hpp"
#include "VertexBuffer/VertexBuffer.hpp"
#include "Shader/ShaderLoader.hpp"
#include <thread>

class Texture;

typedef std::vector<std::pair<std::pair<VertexBuffer, VertexBuffer>, std::pair<VertexBuffer, Texture> > > MODEL_DATA;

class Renderer
{
public:
	void Init();
	void Clear() const;
	void Draw(const VertexBuffer&, const VertexBuffer&, const VertexBuffer&, const Texture&) const;  // vector care contine datele legate de varfuri, normalele varfurilor si coordonatelor de texturare pentru un model

	void cleanup();

	void setVBOArray(MODEL_DATA&);
	MODEL_DATA getVBOArray();

	std::vector<std::string> getFilesNames() const;
	void setNames(std::vector<std::string>&);

	std::vector<bool> getBools() const;

	void addImage(const char*, const char*);

	inline std::thread spawn() { return std::thread([this] { this->Init(); } ); }

private:
	MODEL_DATA m_obj_VBO_array;  // vector care contine datele legate de varfuri, normalele varfurilor si coordonatelor de texturare pentru un model
	std::vector<std::string> m_file_names;
	std::vector<bool> m_to_be_rendered;
};

