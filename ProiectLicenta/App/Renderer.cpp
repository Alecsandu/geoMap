#include "Renderer.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include "ObjectLoader/ObjectLoader.hpp"
#include "Texture/Texture.hpp"
#include "VertexBuffer/VertexBuffer.hpp"

namespace fs = std::filesystem;

void Renderer::Init()
{
	std::vector<std::string> maps_files_name, uv_files_name;
	const char* executable_path = "Miscellaneous";

	for (const auto& entry : fs::directory_iterator(executable_path))
	{
		if (entry.is_regular_file())
		{
			std::string file_path = entry.path().string();
			std::string file_name = file_path.substr(14);
			std::string type = file_name.substr(0, 3);
			if (type.compare("map") == 0)
				maps_files_name.push_back(file_name);
			if (type.compare("uvm") == 0)
				uv_files_name.push_back(file_name);
		}
	}

	std::sort(maps_files_name.begin(), maps_files_name.end());
	std::sort(uv_files_name.begin(), uv_files_name.end());

	std::copy(maps_files_name.begin(), maps_files_name.end(), std::back_inserter(m_file_names));

	const unsigned int length = (maps_files_name.size() <= uv_files_name.size()) ? static_cast<unsigned int>(maps_files_name.size()) : static_cast<unsigned int>(uv_files_name.size());
	for (unsigned int i = 0; i < length; ++i)
	{
		ObjectLoader ob = ObjectLoader((executable_path + std::string("/") + maps_files_name[i]).c_str());

		m_obj_VBO_array.emplace_back(std::make_pair(VertexBuffer(&ob.getVertices()[0], static_cast<unsigned int>(ob.getVertices().size() * sizeof(glm::vec3))),
			VertexBuffer(&ob.getUVs()[0], static_cast<unsigned int>(ob.getUVs().size() * sizeof(glm::vec2)))),
			std::make_pair(VertexBuffer(&ob.getNormals()[0], static_cast<unsigned int>(ob.getNormals().size() * sizeof(glm::vec3))),
				Texture((executable_path + std::string("/") + uv_files_name[i]).c_str())));
	}

	m_to_be_rendered.reserve(m_obj_VBO_array.size() + 50);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexBuffer& vb_vert, const VertexBuffer& vb_uv, const VertexBuffer& vb_normal, const Texture& texture) const
{
	texture.Bind();

	glEnableVertexAttribArray(0);
	vb_vert.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	vb_uv.Bind();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	vb_normal.Bind();
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, vb_vert.getDataSize());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	vb_vert.Unbind();
	vb_uv.Unbind();
	vb_normal.Unbind();
	texture.Unbind();
}

void Renderer::cleanup()
{
	for (decltype(auto) i : m_obj_VBO_array)
	{
		i.first.first.cleanup();
		i.first.second.cleanup();
		i.second.first.cleanup();
		i.second.second.cleanup();
	}
}

void Renderer::setVBOArray(MODEL_DATA& new_array)
{
	this->m_obj_VBO_array = new_array;
}

MODEL_DATA Renderer::getVBOArray()
{
	return this->m_obj_VBO_array;
}

std::vector<std::string> Renderer::getFilesNames() const
{
	return this->m_file_names;
}

void Renderer::setNames(std::vector<std::string>& new_names)
{
	this->m_file_names = new_names;
}

std::vector<bool> Renderer::getBools() const
{
	return this->m_to_be_rendered;
}

void Renderer::addImage(const char* obj_name, const char* dds_name)
{
	ObjectLoader ob = ObjectLoader((std::string("Miscellaneous/") + obj_name).c_str());

	m_obj_VBO_array.emplace_back(std::make_pair(VertexBuffer(&ob.getVertices()[0], static_cast<unsigned int>(ob.getVertices().size() * sizeof(glm::vec3))),
		VertexBuffer(&ob.getUVs()[0], static_cast<unsigned int>(ob.getUVs().size() * sizeof(glm::vec2)))),
		std::make_pair(VertexBuffer(&ob.getNormals()[0], static_cast<unsigned int>(ob.getNormals().size() * sizeof(glm::vec3))),
			Texture((std::string("Miscellaneous/") + dds_name).c_str())));
}
