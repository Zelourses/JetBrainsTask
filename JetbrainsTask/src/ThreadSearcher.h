#ifndef THREAD_SEARCHER_H
#define THREAD_SEARCHER_H
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
class ThreadSearcher {
public:

	void startSearch(std::string& name, std::string& file);
	void startSearch(std::string& name, std::string_view& file);
	void startSearch(std::string& name, const char * file);
	inline bool isSearchFinished() const {
		return isFinished;	//I am not sure about this. Does it creates unnecessary copies or not?
	}
	bool tryToGetData(std::vector<std::string*>& in_vector);
	void stopSearch();
	inline bool isThreadGood() const {
		return isThreadOkay;
	}

private:
	std::vector<std::string*> strings;
	std::string nameToSearch;
	std::string filePath;
	std::mutex searchMutex;
	std::atomic_bool isFinished = true;

	std::jthread searchThread;
	std::atomic_bool shouldThreadKillHimself = false;

	std::atomic_bool isThreadOkay = true;

	void threadSearchFunc();
	
};

#endif