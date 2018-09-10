#pragma once
#include "Typedef.h"
#include <unordered_map>

class ClientManager
{
public:
	ClientManager();

	void addClient(client_ptr);

	void removeClient(client_ptr);

	~ClientManager();

private:
	std::unordered_map<HttpClient*, client_ptr> clients;
};

