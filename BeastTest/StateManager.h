#pragma once
#include "EventManager.h"
#include "TimedRunner.h"
#include "Program.h"

class EventManager;

class StateManager
{
public:
	StateManager(EventManager* evtManager);
	void handleStateMessage(client_ptr, req_ptr);
	void handleConnect(client_ptr, req_ptr);
	void handleDisconnect(client_ptr, req_ptr);
	void updateState(const std::string& processName, const std::string& state);
	~StateManager();

private:
	std::unordered_map<std::string, Program*> programs;
	TimedRunner checkStateRunner;
	EventManager* evtManager;
};
