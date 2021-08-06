#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) :
	m_data_size(size)
{
	glGenBuffers(1, &m_v_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_v_buffer_id);  // bind buffer pentru a crea si initializa datele unui obiect buffer
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind pentru ca nu am nevoie imediat de el
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::cleanup()
{
	glDeleteBuffers(1, &m_v_buffer_id);
}

unsigned int VertexBuffer::getDataSize() const
{
	return this->m_data_size;
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_v_buffer_id);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
