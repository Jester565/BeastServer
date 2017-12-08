#include "ServicePool.h"

ServicePool::ServicePool(int numThreads)
{
	for (int i = 0; i < numThreads; i++) {
		io_service_ptr ioService(new boost::asio::io_service());
		ioServices.push_back(ioService);
	}
	serviceI = 0;
}

void ServicePool::run()
{
	running = true;
	for (int i = 0; i < ioServices.size(); i++) {
		ioServiceRunners.emplace_back(boost::bind(&ServicePool::ioServiceRunFunc, shared_from_this(), ioServices.at(i)));
		ioServiceRunners.at(i).detach();
	}
}

void ServicePool::stop()
{
	for (int i = 0; i < ioServices.size(); i++) {
		ioServices.at(i)->stop();
	}
	running = false;
}

ServicePool::~ServicePool()
{
}

void ServicePool::ioServiceRunFunc(io_service_ptr ioService)
{
	while (true) {
		if (!running) {
			return;
		}
		ioService->run();
	}
}
