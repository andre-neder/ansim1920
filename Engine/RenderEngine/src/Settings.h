#pragma once
#include <iostream>
#include <string>
#include <vector>

class Settings {
public:
	Settings();
	//Graphics
	float m_aspect_ratio;
	int m_window_height;
	int m_window_width;
	int m_MSAA;
	//Render Settings
	bool m_renderShadows;
	bool m_renderBoundingVolumes;
	bool m_renderLights;
	float m_lastTimeStamp;
	//Controls
	

	//Functions
	std::string getWindowName();
private:
	std::string m_window_name;
};