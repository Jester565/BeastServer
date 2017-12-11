#include "StateManager.h"

StateManager::StateManager(EventManager * evtManager)
	:evtManager(evtManager), checkStateRunner(10000)
{

}

void StateManager::handleStateMessage(client_ptr client, req_ptr req)
{

}

void StateManager::handleConnect(client_ptr client, req_ptr req)
{
}

void StateManager::handleDisconnect(client_ptr client, req_ptr req)
{
	
}

void StateManager::updateState(const std::string & processName, const std::string & state)
{
}

StateManager::~StateManager()
{
}
