#include "SSBO.h"

SSBO::SSBO(GLsizeiptr size, GLvoid* data)
{
	glCreateBuffers(1, &m_bufferID);
	glNamedBufferStorage(m_bufferID, size, data, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT /*| GL_MAP_WRITE_BIT*/);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferID);
}
