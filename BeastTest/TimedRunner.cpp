#include "TimedRunner.h"


TimedRunner::TimedRunner(int millisToCall)
	:millisToCall(millisToCall), running(false)
{
	runner = new std::thread(std::bind(&TimedRunner::checkEvents, this));
}

void TimedRunner::addCB(TimedEvent* te)
{
	auto currentMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::unique_lock<std::recursive_mutex> lk(eventMutex);
	events.push_back(std::make_pair(currentMillis, te));
	condVar.set();
}

void TimedRunner::resetCB(TimedEvent* te)
{
	std::unique_lock<std::recursive_mutex> lk(eventMutex);
	for (auto it = events.begin(); it != events.end(); it++) {
		if (it->second == te) {
			it->first = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			auto lastIt = it;
			lastIt++;
			events.splice(events.end(), events, it, lastIt);
			break;
		}
	}
	condVar.set();
}

void TimedRunner::removeCB(TimedEvent* te)
{
	std::unique_lock<std::recursive_mutex> lk(eventMutex);
	for (auto it = events.begin(); it != events.end(); it++) {
		if (it->second == te) {
			events.erase(it);
			return;
		}
	}
}

TimedRunner::~TimedRunner()
{
	delete runner;
	runner = nullptr;
}

void TimedRunner::checkEvents()
{
	std::chrono::milliseconds timeToNextEvent(0);
	while (running) {
		if (timeToNextEvent <= std::chrono::milliseconds(0)) {
			condVar.wait();
		}
		else {
			condVar.wait(timeToNextEvent);
		}
		auto currentMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		
		std::unique_lock<std::recursive_mutex> lk(eventMutex);
		auto it = events.begin();
		for (; it != events.end(); it++) {
			auto timeDif = currentMillis - it->first;
			if (timeDif >= millisToCall) {
				it->second->handler();
				it->first = currentMillis;
			}
			else {
				timeToNextEvent = millisToCall - timeDif;
				events.splice(events.end(), events, events.begin(), it);
				break;
			}
		}
		if (events.empty()) {
			timeToNextEvent = std::chrono::milliseconds(0);
		}
	}
}
