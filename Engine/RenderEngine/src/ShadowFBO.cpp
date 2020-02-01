#include "ShadowFBO.h"

ShadowFBO::ShadowFBO(int type)
{
	m_shadowMapSize = 2048;
	m_type = type;
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	if (type == DIRECTIONAL) {
		createTexture();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTextureID, 0);
		m_depthTextureID_ARB = glGetTextureHandleARB(m_depthTextureID);
		glMakeTextureHandleResidentARB(m_depthTextureID_ARB);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (type == OMNIDIRECTIONAL) {
		createCubeTexture();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTextureID, 0);
		m_depthTextureID_ARB = glGetTextureHandleARB(m_depthTextureID);
		glMakeTextureHandleResidentARB(m_depthTextureID_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || type < 0 || type > 1)
		std::cout << "Error!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowFBO::bindForWriting()
{
	glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void ShadowFBO::bindForWriting(unsigned int face)
{
	glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_depthTextureID, 0);
}

void ShadowFBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint64 ShadowFBO::getHandleARB()
{
	return m_depthTextureID_ARB;
}

void ShadowFBO::createCubeTexture()
{
	glGenTextures(1, &m_depthTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24,
			m_shadowMapSize, m_shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}

void ShadowFBO::createTexture()
{
	glGenTextures(1, &m_depthTextureID);
	glBindTexture(GL_TEXTURE_2D, m_depthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_shadowMapSize, m_shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
