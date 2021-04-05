#include "ThreadSearcher.h"

#include <fstream>
#include <array>
#include <iostream>

static const int AMOUNT_OF_TEMP_ELEMENTS = 30;

void ThreadSearcher::threadSearchFunc() {
	
	shouldThreadKillHimself = false;
	isThreadOkay = true;
	std::string inputString;
	std::ifstream fileStream = std::ifstream(filePath);
	std::array<std::string*, AMOUNT_OF_TEMP_ELEMENTS > found{};
	std::string tmpName = std::string(nameToSearch);
	int i = 0;
	if (fileStream.is_open()) {
		isFinished = false;		
		while (fileStream.good()) {
			fileStream >> inputString;
			if (inputString.find(tmpName) != std::string::npos) {
				found[i] = new std::string(inputString);
				++i;
				if (i == AMOUNT_OF_TEMP_ELEMENTS - 1) {
					searchMutex.lock();
					//std::cout << "We locked" << std::endl;
					for (int j = 0; j < i; ++j) {
						strings.push_back(found[j]);
					}
					searchMutex.unlock();
					i = 0;
				}
			}

		}
		if (i > 0 && !shouldThreadKillHimself) {
			searchMutex.lock();
			for (int j = 0; j < i; ++j) {
				strings.push_back(found[j]);
			}
			searchMutex.unlock();
			isFinished = true;
			return;
		}
		isFinished = true;
		searchMutex.lock();
		for (auto& val : strings)
			delete val;
		strings.clear();
		searchMutex.unlock();
	}
	else {
		isThreadOkay = false;
	}
}

void ThreadSearcher::startSearch(std::string& name, std::string& file) {
	nameToSearch = name;
	filePath = file;
	//This was hard
	searchThread = std::jthread([&](ThreadSearcher* searcher) {
		searcher->threadSearchFunc();
	},this);
}
void ThreadSearcher::startSearch(std::string& name, std::string_view& file) {
	std::string file_string(file);
	startSearch(name,file_string);
}
void ThreadSearcher::startSearch(std::string& name, const char* file) {
	std::string file_string(file);
	startSearch(name, file_string);
}

//inline bool ThreadSearcher::isSearchFinished() const{
//	return isFinished;	//I am not sure about this. Does it creates unnecessary copies or not?
//}

bool ThreadSearcher::tryToGetData(std::vector<std::string*>& in_vector) {
	if (!searchMutex.try_lock()) {
		return false;
	}
	//std::cout << "Wa passed mutex barrier\n";
	for (auto& val : strings) {
		in_vector.push_back(val);
	}
	strings.clear();
	searchMutex.unlock();
	return true;
}

void ThreadSearcher::stopSearch() {
	shouldThreadKillHimself = true;
}


