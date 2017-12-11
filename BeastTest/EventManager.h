#pragma once
#include "Typedef.h"
#include <unordered_set>
#include <unordered_map>
#include <string>

class EventHandler
{
public:
	EventHandler(const std::string& resource, evt_handler handler)
		:resource(resource), handler(handler)
	{
	}

	std::string getResource() {
		return resource;
	}

	evt_handler getHandler() {
		return handler;
	}

private:
	std::string resource;
	evt_handler handler;
};

class EventManager
{
public:
	EventManager();

	void runConnect(client_ptr);
	void runMessage(client_ptr, req_ptr);
	void runDisconnect(client_ptr);

	void runEvent(const std::string& resource, client_ptr, req_ptr);

	void addEventHandler(EventHandler* evtHandler);

	void removeEventHandler(EventHandler* evtHandler);

	~EventManager();

private:
	void onMessageHandlerNotFound(client_ptr, req_ptr);
	
	std::unordered_map<std::string, std::unordered_set<EventHandler*>> eventHandlers;
};

