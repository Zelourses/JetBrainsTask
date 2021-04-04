#ifndef THREAD_SEARCHER_H
#define THREAD_SEARCHER_H
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
class ThreadSearcher {
private:
	std::vector<std::string*> strings;
	std::string nameToSearch;
	std::string filePath;
	std::mutex searchMutex;
	std::atomic_bool isFinished = false;

	std::thread searchThread;


	void threadSearchFunc();
public:
	ThreadSearcher(std::string name, std::string filePath);

	void startSearch();

	inline bool isSearchFinished() const;
	
	bool tryToWriteData(std::vector<std::string*>& in_vector);
	
};

#endif