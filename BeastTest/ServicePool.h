#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

class ServicePool : public boost::enable_shared_from_this<ServicePool>{
public:
	ServicePool(int numThreads);

	void run();

	boost::asio::io_service& getNextIOService() {
		boost::unique_lock<boost::mutex> lock(getServiceMutex);
		boost::asio::io_service& service = *ioServices.at(serviceI);
		serviceI++;
		if (serviceI >= ioServices.size()) {
			serviceI = 0;
		}
		return service;
	}

	void stop();

	~ServicePool();

protected:
	typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;

	void ioServiceRunFunc(io_service_ptr ioService);

	std::vector<boost::thread> ioServiceRunners;

	std::vector<io_service_ptr> ioServices;

	boost::mutex getServiceMutex;

	uint32_t serviceI;

	std::atomic<bool> running;
};