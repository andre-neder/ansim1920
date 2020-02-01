#include "Instance.h"

Instance::Instance()
{
	m_settings = new Settings();
	m_cursorPos.x = m_settings->m_window_width / 2;
	m_cursorPos.y = m_settings->m_window_height / 2;
}

Instance* Instance::getInstance()
{
	if (m_instance == nullptr) 
		m_instance = new Instance();
	return m_instance;
}
