#pragma once
#include "Typedef.h"
#include "ConditionVariable.h"
#include <list>
#include <chrono>

struct TimedEvent {
	TimedEvent(std::function<void()> handler)
		:handler(handler)
	{

	}
	std::function<void()> handler;
};

class TimedRunner
{
public:
	TimedRunner(int millisToCall);

	void addCB(TimedEvent*);
	void resetCB(TimedEvent*);
	void removeCB(TimedEvent*);

	~TimedRunner();

private:
	void checkEvents();
	std::list <std::pair<std::chrono::milliseconds, TimedEvent*>> events;
	std::thread* runner;
	std::recursive_mutex eventMutex;
	ConditionVariable condVar;
	std::chrono::milliseconds millisToCall;
	bool running;
};

