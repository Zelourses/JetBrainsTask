#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>

static int src_width = 1200;
static int src_height = 700;
static const int BUFFER_SIZE = 255;

static void frameBuffer_size_callback(GLFWwindow* window, int width, int height) {
	src_width = width;
	src_height = height;
	glViewport(0, 0, width, height);
}

int main() {
	std::string buffer;
	std::string buffer2;
	buffer.resize(BUFFER_SIZE, '\0');
	buffer2.resize(BUFFER_SIZE,'\0');
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef T_APPLE
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(src_width, src_height, "jetbrainsTask", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to create GLAD context" << std::endl;
		return -2;
}
	glViewport(0, 0, src_width, src_height);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(const_cast<char*>(reinterpret_cast<const char*>(glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS)))); //ugly, and not C style with this (char*) thing
	
	std::vector<std::string*> foundStrings;

	//Main draw loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//-state setting
		glClear(GL_COLOR_BUFFER_BIT);		//--state-using

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::SetNextWindowPos({ 0.0f,0.0f });
		ImGui::SetNextWindowSize({ static_cast<float>(src_width),static_cast<float>(src_height) });
		{
			ImGui::Begin("Text searching");
			ImGui::InputText("Type here!", const_cast<char*>(buffer.c_str()), BUFFER_SIZE);
			ImGui::BeginChild("Search");
			if (buffer != buffer2) {
				auto* newString = new std::string("buffers not equals!");
				foundStrings.push_back(newString);
				buffer2 = buffer;
			}
			for (auto& foundString : foundStrings) {
				//if (!foundString->empty())
					ImGui::Text(foundString->c_str());

			}
			
			ImGui::EndChild();
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
