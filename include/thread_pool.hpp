#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <queue>

class ThreadPool {
public:
	void Start();
	void QueueJob(const std::function<void()>& job);
	void Stop();
	bool busy();
private:
	void ThreadLoop();

	bool should_terminate = false;
	std::mutex queue_mutex;
	std::condition_variable mutex_condition;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> jobs;
};