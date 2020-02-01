#ifndef TEXTURE_H
#define TEXTURE_H
#include <glm/ext.hpp>
#include <GL/glew.h>
#include <iostream>
#include "ShaderSet.h"

class Texture{
public:
	Texture();
	Texture(int width, int height, GLenum format);
	Texture(int width, int height, GLenum format, GLenum type);
	Texture(int width, int height, GLenum format, GLenum type, GLfloat param);
	Texture(int width, int height, GLenum format, GLenum type, GLfloat param, bool generateMipMaps);
	Texture(std::string file);
	~Texture();
	void uploadTexture(ShaderSet* shaderSet, std::string name);
	GLuint getTextureID();
	GLuint64 getHandleARB();
	int getWidth();
	int getHeight();

private:
	int m_width, m_height, m_bytesPerPixel;
	unsigned char *m_data;
	GLuint m_textureID;
	GLuint64 m_handleARB;
};
#endif