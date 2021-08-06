#pragma once
#include <vector>
#include "glm.hpp"
#include <string>
#include <iostream>

class ObjectLoader
{
public:
	ObjectLoader();
	ObjectLoader(const char*);
	~ObjectLoader();
	
	/* getters */
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getUVs();
	std::vector<glm::vec3> getNormals();

	/* setters */
	void setVertices(std::vector<glm::vec3>);
	void setUVs(std::vector<glm::vec2>);
	void setNormals(std::vector<glm::vec3>);

	void cleanup();

private:
	std::vector<glm::vec3> m_out_vertices;
	std::vector<glm::vec2> m_out_uvs;
	std::vector<glm::vec3> m_out_normals;

private:
	bool try_load_object(const char*);
};

