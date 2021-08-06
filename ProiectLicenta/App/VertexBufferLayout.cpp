#include "VertexBufferLayout.hpp"

VertexBufferLayout::VertexBufferLayout()// : m_stride(0) 
{}

VertexBufferLayout::~VertexBufferLayout() {}

void VertexBufferLayout::addFloat(unsigned int count) 
{
    pushVBE(GL_FLOAT, count, GL_FALSE); 
}

void VertexBufferLayout::addUnsignedInt(unsigned int count) 
{ 
    pushVBE(GL_UNSIGNED_INT, count, GL_FALSE);
}

void VertexBufferLayout::addUnsignedByte(unsigned int count) 
{ 
    pushVBE(GL_UNSIGNED_BYTE, count, GL_TRUE); 
}

const std::vector<VertexBufferElement> VertexBufferLayout::getElements() const 
{ 
    return m_buffer_elements;
}

/*unsigned int VertexBufferLayout::getStride() const
{
    return m_stride; 
}*/

/* Introduce elemente vbe pentru a fi mai departe utilizate in glVertexAttribPointer(...) */
void VertexBufferLayout::pushVBE(unsigned int type, unsigned int count, unsigned char normalized)
{
    m_buffer_elements.push_back({ type, count, normalized });
    //m_stride += count * VertexBufferElement::GetSizeOfType(type);
};
