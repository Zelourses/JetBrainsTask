#include "ThreadSearcher.h"

#include <utility>
#include <fstream>
#include <array>
#include <stdexcept>

static const int AMOUNT_OF_TEMP_ELEMENTS = 10;

void ThreadSearcher::threadSearchFunc() {
	std::string inputString;
	auto fileStream = std::fstream(filePath);
	std::array<std::string*, AMOUNT_OF_TEMP_ELEMENTS > found{};
	int i = 0;
	if (fileStream.is_open()) {
		isFinished = false;
		while (fileStream.good()) {
			fileStream >> inputString;
			if (inputString.find(filePath) !=std::string::npos) {
				found[i] = new std::string(inputString);
				++i;
				if (i == AMOUNT_OF_TEMP_ELEMENTS-1) {
					searchMutex.lock();
					for (int j = 0; j < i;++j) {
						strings.push_back(found[j]);
					}
					searchMutex.unlock();
					i = 0;
				}
			}
			if(i>0){
				searchMutex.lock();
				for (int j = 0; j < i; ++j) {
					strings.push_back(found[j]);
				}
				searchMutex.unlock();
			}
			isFinished = true;
		}
		
	}
	else
		throw std::invalid_argument("Problem with file");
}

ThreadSearcher::ThreadSearcher(std::string name, std::string filePath)
	: nameToSearch(std::move(name)), filePath(std::move(filePath)) { }

void ThreadSearcher::startSearch() {
	//This was hard
	searchThread = std::thread([&](ThreadSearcher* searcher) {
		searcher->threadSearchFunc();
	},this);
}

bool ThreadSearcher::isSearchFinished() const{
	return isFinished;	//I am not sure about this. Does it creates unnecessary copies or not?
}

bool ThreadSearcher::tryToWriteData(std::vector<std::string*>& in_vector) {
	if (!searchMutex.try_lock()) {
		return false;
	}
	for (auto& val : strings) {
		in_vector.push_back(val);
	}
	searchMutex.unlock();
	return true;
}


