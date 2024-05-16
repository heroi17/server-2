#pragma once
#ifndef _HEADERS_FABRIC_WORKER_H_
#define _HEADERS_FABRIC_WORKER_H_

#include "../GlobalInclude.h"
#include "threadSafeFixedQueue.h"
#include "fabric.h"

class Worker {

public:
	bool SturtUp(TaskSolverFabric*);

	bool ShutDown();

	bool IsStartingUp() const;

	~Worker();
private:
	void WorkerLoop();

	//FILDS
	std::mutex startUpMutex;
	bool isStartingUp_ = false;
	bool inWorkerLoop = false;
	std::thread myThread_;
	TaskSolverFabric* myFabric_ = nullptr;
};


#endif // !_HEADERS_FABRIC_WORKER_H_
