#pragma once
#include "Typedef.h"
#include <string>

struct TimedEvent;
class StateManager;
class Program
{
public:
	Program(StateManager* sm, client_ptr client);

	void pingCB();
	void pongCB();
	std::string getState() {
		return state;
	}
	void setState(const std::string& state) {
		this->state = state;
	}

	~Program();
private:
	StateManager* stateManager;
	client_ptr client;
	std::string state;
	TimedEvent* pingTE;
	TimedEvent* pongTE;
};

