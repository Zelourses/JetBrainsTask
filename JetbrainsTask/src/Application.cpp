#include "ThreadSearcher.h"
#include "Window.h"

#include <vector>



int main() {
	std::string buffer;
	std::string buffer2;
	buffer.resize(1, '\0');
	buffer2.resize(1,'\0');


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	
	Window workingWindow(io, 1280, 720);
	ThreadSearcher searcher;

	
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(workingWindow.getWindow(), true);
	ImGui_ImplOpenGL3_Init(const_cast<char*>(reinterpret_cast<const char*>(glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS)))); //ugly, and not C style with this (char*) thing
	
	std::vector<std::string*> foundStrings;

	bool canWeCloseWindow = true;
	//float countSecond = 0;

	//Main draw loop
	while (!glfwWindowShouldClose(workingWindow.getWindow())){
		if (buffer != buffer2)
			workingWindow.updateTime();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos({ 0.0f,0.0f });
		ImGui::SetNextWindowSize({ static_cast<float>(workingWindow.getWidth()),
								static_cast<float>(workingWindow.getHeight()) });
		
		ImGui::Begin("Text searching");
		ImGui::InputText("Type here!",&buffer);
		ImGui::SetWindowFontScale(1.3f);
		if (!searcher.isSearchFinished()) {
			ImGui::Text("\tSearching...");
		}else {
			ImGui::Text("\tNothing to search");
		}
		ImGui::BeginChild("Search");
		if (workingWindow.isUserInputIntervalEnded()) {
			canWeCloseWindow = true;
			searcher.stopSearch();
			if (!buffer.empty()){
				searcher.startSearch(buffer, "./words.txt");
				for (auto& val :foundStrings) {
					delete val;
				}
			foundStrings.clear();
			}
			buffer2 = buffer;
			workingWindow.resetInputInterval();
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

		workingWindow.onUpdate();
	}
	return 0;
}
