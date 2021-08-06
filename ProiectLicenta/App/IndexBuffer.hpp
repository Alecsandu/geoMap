#pragma once
#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int*, unsigned int);
	~IndexBuffer();

	void cleanup();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getCount() const { return m_count; }
private:
	unsigned int m_i_buffer_id;
	unsigned int m_count;
};