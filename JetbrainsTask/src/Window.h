#pragma once

#include <algorithm>
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imstb_textedit.h"

class Window {
public:

	void onUpdate();
	void setVsync(bool enabled);

	Window(ImGuiIO& IO, int width, int height);
	GLFWwindow* getWindow() const { return window; }
	static int getWidth();
	static int getHeight();

	void checkKeyEvent();
	void updateTime();
	bool isUserInputIntervalEnded();
	void resetInputInterval() { countSeconds = 0.0f; }

	~Window();
private:
	GLFWwindow* window;
	ImGuiIO& io;
	float countSeconds;
};

