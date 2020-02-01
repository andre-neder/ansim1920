#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <vector>

#define OMNIDIRECTIONAL 1
#define DIRECTIONAL 0
#define SHADOWMAPSIZE 2048

class ShadowFBO {
public:
	ShadowFBO(int type);
	void bindForWriting();
	void bindForWriting(unsigned int face);
	void unbind();
	GLuint64 getHandleARB();
private:
	GLuint m_fbo;
	GLuint m_depthTextureID;
	GLuint64 m_depthTextureID_ARB;
	int m_shadowMapSize;
	int m_type;

	void createCubeTexture();
	void createTexture();

};