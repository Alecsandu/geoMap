#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
	:m_count(count)
{
	glGenBuffers(1, &m_i_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_i_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::cleanup()
{
	glDeleteBuffers(1, &m_i_buffer_id);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_i_buffer_id);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
