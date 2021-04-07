#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ThreadSearcher.h"

#include <iostream>
#include <vector>
#include <stdexcept>

static int src_width = 1200;
static int src_height = 700;

static void frameBuffer_size_callback(GLFWwindow* window, int width, int height) {
	src_width = width;
	src_height = height;
	glViewport(0, 0, width, height);
}

int main() {
	std::string buffer;
	std::string buffer2;
	buffer.resize(1, '\0');
	buffer2.resize(1,'\0');
	
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

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); //vsync
	
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to create GLAD context" << std::endl;
		return -2;
}
	glViewport(0, 0, src_width, src_height);
	
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(const_cast<char*>(reinterpret_cast<const char*>(glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS)))); //ugly, and not C style with this (char*) thing
	
	std::vector<std::string*> foundStrings;

	ThreadSearcher searcher;

	bool canWeCloseWindow = true;
	double countSecond = 0;

	//Main draw loop
	while (!glfwWindowShouldClose(window)){
		if (buffer != buffer2)
			countSecond += 2 * io.DeltaTime;
		glfwPollEvents();
		for (int i = 0; i<512;i++) {
			if (io.KeysDown[i]) {
				countSecond = 0;
			}
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//-state setting
		glClear(GL_COLOR_BUFFER_BIT);		//--state-using

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos({ 0.0f,0.0f });
		ImGui::SetNextWindowSize({ static_cast<float>(src_width),static_cast<float>(src_height) });
		
		ImGui::Begin("Text searching");
		ImGui::InputText("Type here!",&buffer);
		if (!searcher.isSearchFinished()) {
			ImGui::SameLine();
			ImGui::Text("Searching...");
		}
		ImGui::BeginChild("Search");
		if (countSecond > 1.0l) {
			//std::cout << "Buffer value:" << buffer << '\n';
				canWeCloseWindow = true;
				searcher.stopSearch();
				searcher.startSearch(buffer, "words.txt");
			buffer2 = buffer;
			for (auto& val :foundStrings) {
				delete val;
			}
			foundStrings.clear();
			countSecond = 0;
		}
		
		if(searcher.tryToGetData(foundStrings)){
			for (auto& foundString : foundStrings) {
				//if (!foundString->empty())
					ImGui::Text(foundString->c_str());

			}
		}
		
		ImGui::EndChild();
		ImGui::End();
		
		if (!searcher.isThreadGood() && canWeCloseWindow) {
			ImGui::Begin("Oops", &canWeCloseWindow);
			ImGui::Text("We had some problems during file opening");
			ImGui::End();
		}
			
			
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
