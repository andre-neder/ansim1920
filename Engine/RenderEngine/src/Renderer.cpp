#include "Renderer.h"

Renderer::Renderer()
{
	m_instance = m_instance->getInstance();
	glfwInit();
	m_window = glfwCreateWindow(m_instance->m_settings->m_window_width, m_instance->m_settings->m_window_height, m_instance->m_settings->getWindowName().c_str(), 0, 0);
	glfwSetWindowPos(m_window, 600, 50);
	glfwMakeContextCurrent(m_window);
	glewInit();
}

void Renderer::render(Scene* scene)
{
	//scene->render();
}
