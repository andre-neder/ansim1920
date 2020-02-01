#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION  
#include "stb/stb_image.h"

Texture::Texture() {

	}
Texture::Texture(int width, int height, GLenum format)
{
	m_width = width;
	m_height = height;
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	m_handleARB = glGetTextureHandleARB(m_textureID);
	glMakeTextureHandleResidentARB(m_handleARB);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height, GLenum format, GLenum type)
{
	m_width = width;
	m_height = height;
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, type, m_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	m_handleARB = glGetTextureHandleARB(m_textureID);
	glMakeTextureHandleResidentARB(m_handleARB);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height, GLenum format, GLenum type, GLfloat param)
{
	m_width = width;
	m_height = height;
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, type, m_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (param == GL_CLAMP_TO_BORDER) {
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	m_handleARB = glGetTextureHandleARB(m_textureID);
	glMakeTextureHandleResidentARB(m_handleARB);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height, GLenum format, GLenum type, GLfloat param, bool generateMipMaps)
{
	m_width = width;
	m_height = height;
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, type, m_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (param == GL_CLAMP_TO_BORDER) {
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
	if (generateMipMaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	m_handleARB = glGetTextureHandleARB(m_textureID);
	glMakeTextureHandleResidentARB(m_handleARB);
	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::Texture(std::string file) {
	m_data = stbi_load(file.c_str(), &m_width, &m_height, &m_bytesPerPixel, 0);
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	if (m_bytesPerPixel == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, m_data);
	}
	if (m_bytesPerPixel == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	}
	else if (m_bytesPerPixel == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	}
	else {
		std::cout << m_bytesPerPixel << std::endl;
		std::cout << "Couldn't load Texture!" << std::endl;
		std::cout << file << std::endl;
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(m_data);
	m_handleARB = glGetTextureHandleARB(m_textureID);
	glMakeTextureHandleResidentARB(m_handleARB);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	//glDeleteTextures(1, &m_textureID);
}

void Texture::uploadTexture(ShaderSet* shaderSet, std::string name)
{
	glUniform1ui64ARB(glGetUniformLocation(shaderSet->getProgramID(), name.c_str()), m_handleARB);
}

GLuint Texture::getTextureID()
{
	return m_textureID;
}

GLuint64 Texture::getHandleARB()
{
	return m_handleARB;
}

int Texture::getWidth()
{
	return m_width;
}

int Texture::getHeight()
{
	return m_height;
}



