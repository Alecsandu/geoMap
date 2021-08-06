#pragma once
#include <GL/glew.h>

class VertexBuffer
{
public:
	VertexBuffer(const void*, unsigned int);
	~VertexBuffer();

	void cleanup();

	unsigned int getDataSize() const;

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_v_buffer_id;
	unsigned int m_data_size;
};

