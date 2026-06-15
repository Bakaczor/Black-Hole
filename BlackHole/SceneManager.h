#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Camera.h"
#include "Skybox.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

class SceneManager {
	friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	public:
	SceneManager();
	int init();
	int run();

	private:
	Shader skyboxShader;
	Skybox skybox;

	GLFWwindow* window = nullptr;
	std::string title;
	bool cursor = true;

	unsigned int width = 1000;
	unsigned int height = 800;

	Camera camera;
	float lastX = width / 2.0f;
	float lastY = height / 2.0f;
	bool firstMouse = true;

	constexpr static float step = 0.01f;
	float accumTime = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float mass = 1;

	int arrange();
	void processInput();
	void renderUI();
	void renderScene();
	int terminate();
};
#endif