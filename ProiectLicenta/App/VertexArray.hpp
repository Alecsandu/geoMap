#pragma once
#include <GL/glew.h>
#include "VertexBuffer/VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
public:
	VertexArray();
	VertexArray(unsigned int);
	VertexArray(const VertexArray&);
	~VertexArray();

	void cleanup();

	void Bind() const;
	void Unbind() const;

	unsigned int getVertexArrayId();
private:
	unsigned int m_vertex_array_id;
	unsigned int m_size;
};