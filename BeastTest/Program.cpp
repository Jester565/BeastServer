#include "Program.h"
#include "StateManager.h"
#include "TimedRunner.h"


Program::Program(StateManager* sm, client_ptr client)
	:stateManager(sm), client(client)
{
	pingTE = new TimedEvent(std::bind(&Program::pingCB, this));
	pongTE = new TimedEvent(std::bind(&Program::pongCB, this));
}

void Program::pingCB()
{

}

void Program::pongCB()
{

}

Program::~Program()
{
	delete pingTE;
	pingTE = nullptr;
	delete pongTE;
	pongTE = nullptr;
}
