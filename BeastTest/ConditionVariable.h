#pragma once
#include <chrono>
#include <condition_variable>

class ConditionVariable
{
public:
	ConditionVariable(bool ready = false)
		:ready(ready)
	{

	}

	void wait() {
		std::unique_lock<std::mutex> lk(mutex);
		condVar.wait(lk, [&] { return ready; });
		ready = false;
	}

	void wait(std::chrono::milliseconds waitMillis) {
		std::unique_lock<std::mutex> lk(mutex);
		condVar.wait_for(lk, waitMillis, [&] { return ready; });
		ready = false;
	}

	void set() {
		std::lock_guard<std::mutex> lk(mutex);
		ready = true;
		condVar.notify_one();
	}

private:
	std::condition_variable condVar;
	std::mutex mutex;
	bool ready;
};

