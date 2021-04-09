#include "ThreadSearcher.h"

#include <fstream>
#include <array>
#include <iostream>
#include <algorithm>

static const int AMOUNT_OF_TEMP_ELEMENTS = 5; //MORE THAN ZERO

void ThreadSearcher::threadSearchFunc() {
	shouldThreadKillHimself = false;
	isThreadOkay = true;
	std::string inputString;
	std::ifstream fileStream = std::ifstream(filePath);
	std::array<std::string*, AMOUNT_OF_TEMP_ELEMENTS > found{};
	std::string tmpName = std::string(nameToSearch);

	static_assert(AMOUNT_OF_TEMP_ELEMENTS > 0);
	int i = 0; 
	if (fileStream.is_open()) {
		isFinished = false;
		while (fileStream.good() && !shouldThreadKillHimself) {
			fileStream >> inputString;
			if (inputString.find(tmpName) != std::string::npos) {
				found[i] = new std::string(inputString);
				if (i == AMOUNT_OF_TEMP_ELEMENTS - 1) {
					
					searchMutex.lock();
					for (auto j = 0; j < AMOUNT_OF_TEMP_ELEMENTS; ++j) {
						strings.push_back(found[j]);
					}
					searchMutex.unlock();
					/*
					 * If I set this to 0, we need to check if this constant more than max. value
					 * It's useless branch and branch-prediction
					 * Or maybe I'm wrong
					 */
					i = -1;
				}
				++i;
			}

		}
		if (i > 0 && !shouldThreadKillHimself) {
			searchMutex.lock();
			for (auto j = 0; j < i; ++j) {
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
#if __cplusplus > 201703L
	searchThread = std::jthread([&](ThreadSearcher* searcher) {
		searcher->threadSearchFunc();
		}, this);
#else
	if (searchThread.joinable())
		searchThread.detach();
	//This was hard
	searchThread = std::thread([&](ThreadSearcher* searcher) {
		searcher->threadSearchFunc();
	},this);
#endif
}
void ThreadSearcher::startSearch(std::string& name, std::string_view& file) {
	std::string file_string(file);
	startSearch(name,file_string);
}
void ThreadSearcher::startSearch(std::string& name, const char* file) {
	std::string file_string(file);
	startSearch(name, file_string);
}

bool ThreadSearcher::tryToGetData(std::vector<std::string*>& in_vector) {
	if (!searchMutex.try_lock()) {
		return false;
	}
#if __cplusplus > 201703L
	std::ranges::for_each(strings, [&in_vector](auto x) {in_vector.push_back(x); });
#else 
	for(auto& val : strings) {
		in_vector.push_back(val);
	}
#endif
	strings.clear();
	searchMutex.unlock();
	return true;
}

void ThreadSearcher::stopSearch() {
	shouldThreadKillHimself = true;
}

ThreadSearcher::~ThreadSearcher() {
	//searchMutex.unlock();
#if __cplusplus > 201703L
	
#else
	if (searchThread.joinable())
		searchThread.join();
#endif
}


