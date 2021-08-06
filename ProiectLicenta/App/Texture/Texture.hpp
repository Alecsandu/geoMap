#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	Texture();
	Texture(const char*);
	~Texture();

	void cleanup();

	void setTexture(unsigned int);
	unsigned int getTexture();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_texture_id;
};