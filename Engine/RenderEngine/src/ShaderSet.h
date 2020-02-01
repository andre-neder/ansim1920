#ifndef SHADERSET_H
#define SHADERSET_H
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
class ShaderSet
{
public:
	ShaderSet();
	ShaderSet(GLuint shader_mask, const char** ShaderNames);
	~ShaderSet();

	void GenerateShaderProgramm(GLuint shader_mask, const char** ShaderNames);
	void UseProgram();

	GLuint getProgramID();

private:
	void checkShader(GLuint shaderID);
	void checkProgram(GLuint programID);
	void loadShaderSource(GLint shaderID, const char* fileName);

	GLuint m_shader_mask;
	const char* m_fileName;

protected:
	GLuint m_ProgramID;
};

#endif