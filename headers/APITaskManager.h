#pragma once
#ifndef _HEADERS_TASKMANAGER_H_
#define _HEADERS_TASKMANAGER_H_

#include "GlobalInclude.h"
#include "Fabric/fabric.h"


struct APITaskManager {

	bool INIT();

	bool DISABLE();

	bool StartUp();

	bool ShutDown();

	NewTaskContainerError NewTaskContainer(UNIC_KEY&);

	OpperationResult StoreTask(TASK_CONTAINER&);

	OpperationResult LoadTaskAnswer(UNIC_KEY);

	OpperationResult FreeTaskAnswer(UNIC_KEY);


};













#endif // !_HEADERS_TASKMANAGER_H_

