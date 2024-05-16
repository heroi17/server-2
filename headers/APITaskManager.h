#pragma once
#ifndef _HEADERS_TASKMANAGER_H_
#define _HEADERS_TASKMANAGER_H_

#include "GlobalInclude.h"
#include "Fabric/fabric.h"

//just wrapper around fabric_ with hash. (if people want to change other's data)
struct APITaskManager {
private:
	using OpRes = OpperationResult;
public:
	MANAGER_INIT_ERROR INIT(size_t answersCount, size_t tasksCount, size_t workersCount);

	bool DISABLE();

	bool StartUp();

	bool ShutDown();

	bool APINewTaskContainer(UNIC_KEY& key);

	OpperationResult APIStoreTask(const TASK_CONTAINER& taskContainer, UNIC_KEY key);

	OpperationResult APILoadTaskAnswer(TASK_ANSWER& answer,UNIC_KEY key);

	OpperationResult APIFreeTaskAnswer(UNIC_KEY key);
private:

	OpperationResult ValidateKey(UNIC_KEY key, TASK_ID& taskId);

	TaskSolverFabric fabric_;

};













#endif // !_HEADERS_TASKMANAGER_H_

