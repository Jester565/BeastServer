#include "ClientManager.h"



ClientManager::ClientManager()
{
}

void ClientManager::addClient(client_ptr client)
{
	clients.insert(std::make_pair(client.get(), client));
}

void ClientManager::removeClient(client_ptr client)
{
	clients.erase(client.get());
}


ClientManager::~ClientManager()
{
}
