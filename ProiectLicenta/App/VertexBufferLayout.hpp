#pragma once
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
        }
        return 0;
    }
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();
	~VertexBufferLayout();

    void addFloat(unsigned int);
    void addUnsignedInt(unsigned int);
    void addUnsignedByte(unsigned int);

    const std::vector<VertexBufferElement> getElements() const;
    //unsigned int getStride() const;

private:
    //unsigned int m_stride;

    std::vector<VertexBufferElement> m_buffer_elements;
    void pushVBE(unsigned int, unsigned int, unsigned char);
};

