#include "EventManager.h"
#include "HttpClient.h"

using namespace boost::beast;

EventManager::EventManager()
{
}

void EventManager::runConnect(client_ptr client)
{
	runEvent("connect", client, nullptr);
}

void EventManager::runMessage(client_ptr client, req_ptr req)
{
	runEvent((std::string)req->target(), client, req);
}

void EventManager::runDisconnect(client_ptr client)
{
	runEvent("disconnect", client, nullptr);
}

void EventManager::runEvent(const std::string & resource, client_ptr client, req_ptr req)
{
	auto it = eventHandlers.find(resource);
	if (it != eventHandlers.end()) {
		for (auto subIt = it->second.begin(); subIt != it->second.end(); it++) {
			(*subIt)->getHandler()(client, req);
		}
	}
	else
	{
		onMessageHandlerNotFound(client, req);
	}
}

void EventManager::addEventHandler(EventHandler* evtHandler)
{
	auto it = eventHandlers.find(evtHandler->getResource());
	if (it == eventHandlers.end()) {
		std::unordered_set<EventHandler*> set;
		set.insert(evtHandler);
		eventHandlers.insert(std::make_pair(evtHandler->getResource(), set));
	}
	else {
		it->second.insert(evtHandler);
	}
}

void EventManager::removeEventHandler(EventHandler* evtHandler) {
	auto it = eventHandlers.find(evtHandler->getResource());
	if (it != eventHandlers.end()) {
		it->second.erase(evtHandler);
	}
}

EventManager::~EventManager()
{
}


void EventManager::onMessageHandlerNotFound(client_ptr client, req_ptr req)
{
	if (req == nullptr) {
		return;
	}
	auto resp = CreateResponse();
	resp->result(404);
	resp->keep_alive(true);
	resp->set(http::field::server, "Beast");
	resp->set(http::field::content_type, "text/plain");
	resp->body() = "Could not find resource " + (std::string)req->target();
	client->send(resp);
}
