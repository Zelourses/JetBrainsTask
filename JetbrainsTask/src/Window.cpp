#include "Window.h"

static const int INTERVAL_SECONDS = 2.0f;

int src_width, src_height;
/*
 * GLFWwindow is needed for correct work of resize callback
 */
static void resizeCallback(GLFWwindow* window, int width, int height){
	src_width = width;
	src_height = height;
	glViewport(0, 0, width, height);
}

void Window::onUpdate() {
	glfwSwapBuffers(window);
	glfwPollEvents();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//-state setting
	glClear(GL_COLOR_BUFFER_BIT);		//--state-using
}

void Window::setVsync(bool enabled) {
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

Window::Window(ImGuiIO& IO, int width = 1280, int height = 720)
	: io(IO), countSeconds(0) {

	static_assert(INTERVAL_SECONDS > 0);
	
	src_width = width;
	src_height = height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef T_APPLE
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	window = glfwCreateWindow(width, height, "JetBrainsTask", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	setVsync(true);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glViewport(0, 0, src_width, src_height);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
}

int Window::getWidth() {
	return src_width;
}

int Window::getHeight() {
	return src_height;
}

void Window::checkKeyEvent() {
	for (bool i : io.KeysDown) {
		if (i) {
			resetInputInterval();
			return;
		}
	}
}

void Window::updateTime() {
	countSeconds += INTERVAL_SECONDS * io.DeltaTime;
}

bool Window::isUserInputIntervalEnded() {
	return countSeconds > INTERVAL_SECONDS;
}

Window::~Window() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}




