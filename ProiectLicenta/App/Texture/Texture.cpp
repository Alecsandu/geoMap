#include "Texture.hpp"
#include <iostream>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

Texture::Texture() : m_texture_id(0)
{}

Texture::Texture(const char* ruta_fisier_dds)
{
	m_texture_id = 0;
	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fp = fopen(ruta_fisier_dds, "rb");
	if (fp == NULL) {
		printf("%s nu a putut fi deschis. Verificati daca fisierele se afla in folderul corect!\n", ruta_fisier_dds);
		return;
	}

	/* verify the type of file */
	char filecode[4] = { 0 };
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}

	/* get the surface desc */
	fread(header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* incarac mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// pentru texturile care nu au dimensiuni care nu se impart la 2
		if (width < 1)
			width = 1;
		if (height < 1)
			height = 1;
	}
	free(buffer);
	
	m_texture_id = textureID;
}

Texture::~Texture()
{
}

void Texture::cleanup()
{
	glDeleteTextures(1, &m_texture_id);
}

void Texture::setTexture(unsigned int tex)
{
	this->m_texture_id = tex;
}

unsigned int Texture::getTexture()
{
	return this->m_texture_id;
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
