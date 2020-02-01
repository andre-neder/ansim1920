#include "Settings.h"

Settings::Settings()
{
	m_window_name = "Engine";
	m_aspect_ratio = 16.0f / 9;
	m_window_height = 720;
	m_window_width = (int)(m_aspect_ratio * m_window_height);
	m_renderBoundingVolumes = false;
	m_renderShadows = true;
	m_renderLights = false;
	m_MSAA = 4;

}

std::string Settings::getWindowName()
{
	return m_window_name;
}
