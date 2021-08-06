#include "VertexArray.hpp"
#include <iostream>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_vertex_array_id);
	m_size = 1;
}

VertexArray::VertexArray(unsigned int size)
{
	m_size = size;
	glGenVertexArrays(size, &m_vertex_array_id);
}

VertexArray::VertexArray(const VertexArray& copy_va)
{
	m_size = copy_va.m_size;
	m_vertex_array_id = copy_va.m_vertex_array_id;
	std::cout << "Copied" << m_vertex_array_id << std::endl;
}

VertexArray::~VertexArray()
{
}

void VertexArray::cleanup()
{
	glDeleteVertexArrays(m_size, &m_vertex_array_id);
}

unsigned int VertexArray::getVertexArrayId()
{
	return this->m_vertex_array_id;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_vertex_array_id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
