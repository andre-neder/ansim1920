#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ShaderSet.h"
#include "Scene.h"
#include "Lightsource.h"
#include "Camera.h"
#include "Instance.h"
#include "../vendor/include/imgui/imgui.h"
#include "../vendor/include/imgui/imgui_impl_glfw_gl3.h"
#include "Defines.h"
#include "physics/RigidBodyPhysics.h"

Instance* Instance::m_instance = 0;
Instance* instance = instance->getInstance();

Camera* camera = new Camera(instance->m_settings->m_aspect_ratio);
Scene* scene = nullptr;

RigidBodyPhysics* rbp = new RigidBodyPhysics();

float lastTimeKeys = 0;
//Simulation
bool simulationStarted = false;

void resizeCallBack(GLFWwindow* window, int width, int height) {
	camera->m_aspectRatio = (float)width / height;
	camera->updateProjMatrix();
	instance->m_settings->m_window_height = height;
	instance->m_settings->m_window_width = width;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->m_radius -= 0.1f * yoffset;
}

void mouseCallback(GLFWwindow* window) {
	double x, y;
	glm::vec2 oldPos = instance->m_cursorPos;
	glfwGetCursorPos(window, &x, &y);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && ImGui::IsAnyWindowFocused() == false){
		float changeX = ((float)x - oldPos.x) / instance->m_settings->m_window_width * 2 * glm::pi<float>() ;
		float changeY = ((float)y - oldPos.y) / instance->m_settings->m_window_height * glm::pi<float>();

		camera->m_angles.x -= changeY;
		if (camera->m_angles.x < 0.01f) camera->m_angles.x = 0.01f;
		else if (camera->m_angles.x > glm::pi<float>() - 0.01f) camera->m_angles.x = glm::pi<float>() - 0.01f;

		camera->m_angles.y -= changeX;
		if (camera->m_angles.y < 0) camera->m_angles.y += 2 * glm::pi<float>();
		else if (camera->m_angles.y > 2 * glm::pi<float>()) camera->m_angles.y -= 2 * glm::pi<float>();
	}
	oldPos.x = (float)x;
	oldPos.y = (float)y;
	instance->m_cursorPos = oldPos;
	camera->m_camera.eye.x = camera->m_center.x + camera->m_radius * sin(camera->m_angles.x) * sin(camera->m_angles.y);
	camera->m_camera.eye.y = camera->m_center.y + camera->m_radius * cos(camera->m_angles.x);
	camera->m_camera.eye.z = camera->m_center.z + camera->m_radius * sin(camera->m_angles.x) * cos(camera->m_angles.y);

	camera->m_camera.viewMatrix = glm::lookAt(camera->m_camera.eye, camera->m_center, camera->m_up);
}

void keyCallback(GLFWwindow* window) {
	float deltaTime = glfwGetTime() - lastTimeKeys;
	float speed = 2.0;
	glm::vec3 view = camera->m_center - camera->m_camera.eye;
	glm::vec3 up = camera->m_up;
	glm::vec3 right = glm::normalize(glm::cross(view, up));
	glm::vec3 globalUp = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 front = glm::normalize(glm::cross(globalUp, right));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->m_center += front * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->m_center += right * -deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->m_center += front * -deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->m_center += right * deltaTime * speed;
	}
	/*if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.m_center += globalUp * -deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.m_center += globalUp * deltaTime;
	}*/
	camera->m_camera.viewMatrix = glm::lookAt(camera->m_camera.eye, camera->m_center, camera->m_up);
	lastTimeKeys = glfwGetTime();
}

void sceneInit() {
	scene = new Scene();
	double startinit = glfwGetTime();
	std::cout << "Initialising Scene... ";
	//Camera
	scene->setCamera(camera);
	//Meshes
	Mesh* cubeMesh = new Mesh(MODEL_PATH"plane.obj");
	Mesh* sphereMesh = new Mesh(MODEL_PATH"sphere.obj");
	Mesh* suzanneMesh = new Mesh(MODEL_PATH"suzanne.obj");
	//Materials
	Material* billard_0 = new Material(TEXTURE_PATH"billard_8.png", "", "", "", "");
	Material* billard_1 = new Material(TEXTURE_PATH"billard_1.png", "", "", "", "");
	Material* billard_2 = new Material(TEXTURE_PATH"billard_2.png", "", "", "", "");
	Material* billard_3 = new Material(TEXTURE_PATH"billard_3.png", "", "", "", "");
	Material* billard_4 = new Material(TEXTURE_PATH"billard_4.png", "", "", "", "");
	Material* billard_5 = new Material(TEXTURE_PATH"billard_5.png", "", "", "", "");
	Material* billard_6 = new Material(TEXTURE_PATH"billard_6.png", "", "", "", "");
	Material* billard_7 = new Material(TEXTURE_PATH"billard_7.png", "", "", "", "");
	Material* blue = new Material(glm::vec3(1.0/255.0,50.0/255.0,211.0/255.0), 1.0, 0.0);
	//Objects

	float billardTableWidth = 122.0 / 5.715;
	float billardTableLength = 214.0 / 5.715;
	float billardBallRadius = 1.0; 
	float billardBallMass = 0.1701 * 5.715;

	//Balls

	Object* ball000 = new Object("ball000", sphereMesh, billard_0);
	ball000->setTransforms(glm::vec3(0.0, 1.0, 20.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball000);
	RigidBody* rs2 = new DynamicBody(ball000, SPHERE, billardBallMass);
	std::cout << rs2->getMass() << std::endl;
	rbp->addRigidBody(rs2);
	rs2->setInitialVelocity(glm::vec3(0.0, 0.0, -14.0));
	//rs2->setInitialAngularVelocity(glm::vec3(10.0, 0.0, 0.0));

	Object* ball001 = new Object("ball001",sphereMesh, billard_1);
	ball001->setTransforms(glm::vec3(0.0,1.0,-5.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball001);
	rbp->addRigidBody(new DynamicBody(ball001, SPHERE, billardBallMass));

	Object* ball003 = new Object("ball003", sphereMesh, billard_3);
	ball003->setTransforms(glm::vec3(-1.5, 1.0, -7.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball003);
	rbp->addRigidBody(new DynamicBody(ball003, SPHERE, billardBallMass));

	Object* ball004 = new Object("ball004", sphereMesh, billard_4);
	ball004->setTransforms(glm::vec3(5.2, 1.0, -20.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball004);
	rbp->addRigidBody(new DynamicBody(ball004, SPHERE, billardBallMass));

	Object* ball005 = new Object("ball005", sphereMesh, billard_5);
	ball005->setTransforms(glm::vec3(-4.2, 1.0, -6.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball005);
	rbp->addRigidBody(new DynamicBody(ball005, SPHERE, billardBallMass));

	Object* ball006 = new Object("ball006", sphereMesh, billard_6);
	ball006->setTransforms(glm::vec3(5.0, 1.0, -3.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball006);
	rbp->addRigidBody(new DynamicBody(ball006, SPHERE, billardBallMass));

	Object* ball007 = new Object("ball007", sphereMesh, billard_7);
	ball007->setTransforms(glm::vec3(-7.0, 1.0, -10.0), glm::vec3(0.0), glm::vec3(1.0));
	scene->addObject(ball007);
	rbp->addRigidBody(new DynamicBody(ball007, SPHERE, billardBallMass));


	//Billardtable
	Object* plane001 = new Object("plane001",cubeMesh, blue);
	plane001->setTransforms(glm::vec3(0.0, -0.0, 0.0), glm::vec3(0.0,0.0,0.0), glm::vec3(billardTableWidth, 1.0, billardTableLength));
	scene->addObject(plane001);
	rbp->addRigidBody(new StaticBody(plane001, PLANE));

	Object* plane002 = new Object("plane002", cubeMesh, blue);
	plane002->setTransforms(glm::vec3(0.0, 1.0, billardTableLength), glm::vec3(-90.0, 0.0, 0.0), glm::vec3(billardTableWidth, 1.0, 1.0));
	scene->addObject(plane002);
	rbp->addRigidBody(new StaticBody(plane002, PLANE));

	Object* plane003 = new Object("plane003", cubeMesh, blue);
	plane003->setTransforms(glm::vec3(0.0, 1.0, -billardTableLength), glm::vec3(90.0, 0.0, 0.0), glm::vec3(billardTableWidth, 1.0, 1.0));
	scene->addObject(plane003);
	rbp->addRigidBody(new StaticBody(plane003, PLANE));
	
	Object* plane004 = new Object("plane004", cubeMesh, blue);
	plane004->setTransforms(glm::vec3(-billardTableWidth, 1.0, 0.0), glm::vec3(0.0, 0.0, -90.0), glm::vec3(1.0, 1.0, billardTableLength));
	scene->addObject(plane004);
	rbp->addRigidBody(new StaticBody(plane004, PLANE));
	
	Object* plane005 = new Object("plane005", cubeMesh, blue);
	plane005->setTransforms(glm::vec3(billardTableWidth, 1.0, 0.0), glm::vec3(0.0, 0.0, 90.0), glm::vec3(1.0, 1.0, billardTableLength));
	scene->addObject(plane005);
	rbp->addRigidBody(new StaticBody(plane005, PLANE));
	
	//Lights
	Lightsource* light001 = new Lightsource(glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.0, 10.0, 0.0, 1.0), glm::vec2(0.0, 0.0));
	scene->addLight("light001",light001);
	Lightsource* light002 = new Lightsource(glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.0, 10.0, -20.0, 1.0), glm::vec2(0.0, 0.0));
	scene->addLight("light002", light002);
	Lightsource* light003 = new Lightsource(glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.0, 10.0, 20.0, 1.0), glm::vec2(0.0, 0.0));
	scene->addLight("light003", light003);

	//Scene
	scene->prepareScene();

	std::cout << "Done (" << glfwGetTime() - startinit << " s)" << std::endl;
}


void MainMenuBar(GLFWwindow* window)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "")) {
				glfwSetWindowShouldClose(window, 1);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::BeginMenu("Render"))
			{
				ImGui::Checkbox("Render Shadows", &instance->m_settings->m_renderShadows);
				ImGui::Checkbox("Render Bounding Volumes", &instance->m_settings->m_renderBoundingVolumes);
				ImGui::Checkbox("Render Lights", &instance->m_settings->m_renderLights);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

 void ShowSceneGraph()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Rigid Bodys"))
	{
		// left
		static int selected = 0;
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		std::vector<RigidBody*>* rigidBodys = rbp->getRigidBodyArray();
		for (int i = 0; i < rigidBodys->size(); i++)
		{
			char label[128];
			sprintf(label, rigidBodys->at(i)->getObject()->getName().c_str(), i);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}
		ImGui::EndChild();
		ImGui::SameLine();

		// right
		ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
				ImGui::Text(rigidBodys->at(selected)->getObject()->getName().c_str(), selected);
				ImGui::Separator();
				std::stringstream ss;
				ss << "Mass: " << rigidBodys->at(selected)->getMass();
				ImGui::Text(ss.str().c_str(), selected);
				ss.str("");
				ImGui::Columns(2);
				ss << "Linear Velocity:";
				ss << "\nX: " << rigidBodys->at(selected)->getLinearVelocity().x << "m/s ";
				ss << "\nY: " << rigidBodys->at(selected)->getLinearVelocity().y << "m/s ";
				ss << "\nZ: " << rigidBodys->at(selected)->getLinearVelocity().z << "m/s ";
				ImGui::Text(ss.str().c_str(), selected);
				ss.str("");
				ImGui::NextColumn();
				ss << "Angular Velocity:";
				ss << "\nX: " << rigidBodys->at(selected)->getAngularVelocity().x << "m/s ";
				ss << "\nY: " << rigidBodys->at(selected)->getAngularVelocity().y << "m/s ";
				ss << "\nZ: " << rigidBodys->at(selected)->getAngularVelocity().z << "m/s ";
				ImGui::Text(ss.str().c_str(), selected);
				ss.str("");
				ImGui::NextColumn();
				ss << "Applied Force:";
				ss << "\nX: " << rigidBodys->at(selected)->getForce().x << " kg m/s^2 ";
				ss << "\nY: " << rigidBodys->at(selected)->getForce().y << " kg m/s^2 ";
				ss << "\nZ: " << rigidBodys->at(selected)->getForce().z << " kg m/s^2 ";
				ImGui::Text(ss.str().c_str(), selected);
				ss.str("");
				ImGui::NextColumn();
				ss << "Rotation Matrix";
				ss << "\n" << rigidBodys->at(selected)->getShape()->getRotation()[0].x << "\t" << rigidBodys->at(selected)->getShape()->getRotation()[0].y << "\t" << rigidBodys->at(selected)->getShape()->getRotation()[0].z;
				ss << "\n" << rigidBodys->at(selected)->getShape()->getRotation()[1].x << "\t" << rigidBodys->at(selected)->getShape()->getRotation()[1].y << "\t" << rigidBodys->at(selected)->getShape()->getRotation()[1].z;
				ss << "\n" << rigidBodys->at(selected)->getShape()->getRotation()[2].x << "\t" << rigidBodys->at(selected)->getShape()->getRotation()[2].y << "\t" << rigidBodys->at(selected)->getShape()->getRotation()[2].z;
				ImGui::Text(ss.str().c_str(), selected);
				ss.str("");
			ImGui::EndChild();
		ImGui::EndGroup();
	}
	ImGui::End();
}

 

int main() {
	//glfw init -> Renderer();
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(instance->m_settings->m_window_width, instance->m_settings->m_window_height, instance->m_settings->getWindowName().c_str(), 0, 0);
	glfwSetWindowPos(window, 600, 50);
	glfwMakeContextCurrent(window);
	//Controls init
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowSizeCallback(window, resizeCallBack);
	//glew init -> Renderer();
	glewInit();
	//Renderer Settings
	glfwWindowHint(GLFW_SAMPLES, instance->m_settings->m_MSAA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glfwSwapInterval(0);

	//Imgui Init
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, false);
	ImGui::StyleColorsDark();
	

	//Shaders
	const char* shaderfiles[2] = { SHADER_PATH "pbr.vert",SHADER_PATH"pbr.frag" };
	ShaderSet pbr(1 | 16, shaderfiles);

	const char* shaderfiles2[2] = { SHADER_PATH"shadowMap.vert",SHADER_PATH"shadowMap.frag" };
	ShaderSet shadowMap(1 | 16, shaderfiles2);

	const char* shaderfiles3[2] = { SHADER_PATH"minimal.vert",SHADER_PATH"boundingVolume.frag" };
	ShaderSet boundingVolume(1 | 16, shaderfiles3);

	const char* shaderfiles4[2] = { SHADER_PATH"minimal.vert",SHADER_PATH"lightSource.frag" };
	ShaderSet lightSource(1 | 16, shaderfiles4);
		
	sceneInit();


	//Time
	double t = 0.0;
	double dt = 0.01;
	float speedOfTime = 1.0;
	double currentTime = glfwGetTime();
	double accumulator = 0.0;
	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		int fps = 1.0 / frameTime;
		if (frameTime > 0.25)
			frameTime = 0.25;
		currentTime = newTime;

		accumulator += frameTime;
		ImGui_ImplGlfwGL3_NewFrame();
		mouseCallback(window);
		keyCallback(window);
		//Physics Loop
		while (accumulator >= dt)
		{
			t += dt;
			accumulator -= dt;
			if (simulationStarted)
				rbp->update(speedOfTime*dt);
		}

		if (scene != nullptr) {
			//Render Shadows
			if (instance->m_settings->m_renderShadows == true)
				scene->renderShadows(&shadowMap);

			//Render Scene
			scene->render(&pbr);

			//Render Bounding Volumes
			if (instance->m_settings->m_renderBoundingVolumes == true)
				scene->renderBoundingVolumes(&boundingVolume);

			//Render Light Models
			if (instance->m_settings->m_renderLights == true)
				scene->renderLights(&lightSource);
		}
		//ImGui
	

		MainMenuBar(window);
		ShowSceneGraph();

		//ImGui::Begin("Physics");
		//ImGui::SliderFloat3("Gravity", glm::value_ptr(rbp->m_gravity), -100.0, 100.0,"%.2fm/s^2", 1.0);
		//ImGui::End();

		ImGui::Begin("Simulation");
		if (!simulationStarted) {
			if (ImGui::Button("Start Simulation", ImVec2(150, 30)))
				simulationStarted = true;
		}
		else {
			if (ImGui::Button("Pause Simulation", ImVec2(150, 30)))
				simulationStarted = false;
		}
		ImGui::SliderFloat("Speed of Time", &speedOfTime, 0.125, 4.0);
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		const double alpha = accumulator / dt;
	}

	scene->~Scene();
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}