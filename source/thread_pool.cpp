#include "thread_pool.hpp"

void ThreadPool::Start() {
	const uint32_t num_threads = std::thread::hardware_concurrency();
	for (uint32_t i = 0; i < num_threads; ++i) {
		threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
	}
}

void ThreadPool::ThreadLoop() {
	while (true) {
		std::function<void()> job;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			mutex_condition.wait(lock, [this] {
				return !jobs.empty() || should_terminate;
			});
			if (should_terminate) {
				return;
			}
			job = jobs.front();
			jobs.pop();
		}
		job();
	}
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		jobs.push(job);
	}
	mutex_condition.notify_one();
}

bool ThreadPool::busy() {
	bool poolbusy;
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		poolbusy = !jobs.empty();
	}
	return poolbusy;
}

void ThreadPool::Stop() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		should_terminate = true;
	}
	mutex_condition.notify_all();
	for (auto& active_thread : threads) {
		active_thread.join();
	}
	threads.clear();
}