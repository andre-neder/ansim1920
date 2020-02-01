#include "ShaderSet.h"

ShaderSet::ShaderSet()
{

}

// ShaderNames must be in order Vertx, Tess_Control, Tess_Eval, Geometry, Fragment, Compute
ShaderSet::ShaderSet(GLuint shader_mask, const char** ShaderNames)
{
	GenerateShaderProgramm(shader_mask, ShaderNames);
}

GLuint ShaderSet::getProgramID()
{
	return(m_ProgramID);
}

ShaderSet::~ShaderSet()
{
	if (m_ProgramID != 0xFFFFFFFF) glDeleteProgram(m_ProgramID);
}

void ShaderSet::GenerateShaderProgramm(GLuint shader_mask, const char** ShaderNames)
{
	GLuint vertexShaderID, tessControlShaderID, tessEvalShaderID;
	GLuint geometryShaderID, fragmentShaderID, computeShaderID;

	int next_name = 0;
	m_shader_mask = shader_mask;

	m_ProgramID = 0xFFFFFFFF;

	if (shader_mask & 1)
	{
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		loadShaderSource(vertexShaderID, ShaderNames[next_name++]);
		glCompileShader(vertexShaderID);
		checkShader(vertexShaderID);
	}

	if (shader_mask & 2)
	{
		tessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		loadShaderSource(tessControlShaderID, ShaderNames[next_name++]);
		glCompileShader(tessControlShaderID);
		checkShader(tessControlShaderID);
	}

	if (shader_mask & 4)
	{
		tessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		loadShaderSource(tessEvalShaderID, ShaderNames[next_name++]);
		glCompileShader(tessEvalShaderID);
		checkShader(tessEvalShaderID);
	}

	if (shader_mask & 8)
	{
		geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		loadShaderSource(geometryShaderID, ShaderNames[next_name++]);
		glCompileShader(geometryShaderID);
		checkShader(geometryShaderID);
	}

	if (shader_mask & 16)
	{
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		loadShaderSource(fragmentShaderID, ShaderNames[next_name++]);
		glCompileShader(fragmentShaderID);
		checkShader(fragmentShaderID);
	}

	if (shader_mask & 32)
	{
		computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
		loadShaderSource(computeShaderID, ShaderNames[next_name++]);
		glCompileShader(computeShaderID);
		checkShader(computeShaderID);
	}

	//link shader programs
	m_ProgramID = glCreateProgram();

	if (shader_mask & 1) glAttachShader(m_ProgramID, vertexShaderID);
	if (shader_mask & 2) glAttachShader(m_ProgramID, tessControlShaderID);
	if (shader_mask & 4) glAttachShader(m_ProgramID, tessEvalShaderID);
	if (shader_mask & 8) glAttachShader(m_ProgramID, geometryShaderID);
	if (shader_mask & 16) glAttachShader(m_ProgramID, fragmentShaderID);
	if (shader_mask & 32) glAttachShader(m_ProgramID, computeShaderID);

	glLinkProgram(m_ProgramID);
	checkProgram(m_ProgramID);
}

void ShaderSet::UseProgram()
{
	glUseProgram(m_ProgramID);
}

//checks a shader for compiler errors
void ShaderSet::checkShader(GLuint shaderID) {
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);

		std::cout << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
		delete[] infoLog;
	}
}

//checks a program
void ShaderSet::checkProgram(GLuint programID) {
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);

		std::cout << "ERROR: Unable to link ShaderSet" << std::endl << infoLog << std::endl;
		delete[] infoLog;
	}
}

//reads a file and returns the content as a pointer to chars
void ShaderSet::loadShaderSource(GLint shaderID, const char* fileName) {
	std::string fileContent;
	std::string line;
	m_fileName = fileName;
	//open file and "parse" input
	std::ifstream file(fileName);
	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			fileContent += line + "\n";
		}
		file.close();
	}
	else
		std::cout << "ERROR: Unable to open file " << fileName << std::endl;

	const char* source = fileContent.c_str();
	const GLint source_size = strlen(source);

	glShaderSource(shaderID, 1, &source, &source_size);
}
