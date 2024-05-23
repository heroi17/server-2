#pragma once
#ifndef _HEADERS_FABRIC_FABRIC_H_
#define _HEADERS_FABRIC_FABRIC_H_



#include "../GlobalInclude.h"
#include "threadSafeFixedQueue.h"
#include "AnswerManager.h"
#include "worker.h"


class TaskSolverFabric {
	friend class Worker;

public:
	MANAGER_INIT_ERROR INIT(uint32_t answersCount, uint32_t tasksCount, uint32_t workersCount);

	//return false if already init;
	bool DISABLE();

	//return false if already startingUp or not init
	bool FabricStartUp();

	bool FabricShutDown();



	bool allocateSpaceForTask(TASK_ID&);

	//store task into this Fabric
	bool storeTaskAndStartSolving(const TASK_CONTAINER&, TASK_ID);

	// start if task is stored into TAS_ID and TASK_ID is exist

	//return true if already solved and set AnswerContainer* to answerContainer;
	bool getSolvedAnswerContainer(AnswerContainer&, TASK_ID);

	//can free answer if not state = WAITING_IN_QUEUE_FOR_SOLWE or WAITING_IN_QUEUE_FOR_SOLWE
	bool freeAnswerContainer(TASK_ID);


	// return false if task with this id is not exist
	bool getTaskState(TASK_STATE& , TASK_ID);

	// return false if task with this id is not exist
	bool getTaskHash(uint32_t&, TASK_ID);

	~TaskSolverFabric();
private:

	//Fields


	mutable std::recursive_mutex accessMutex;
	bool isStartingUp = false;
	bool isInit = false;
	std::list<Worker> workers_; //not vector, because i can't move workers(none copy mutex). but maybe workers will be far away from each other and it's not so good.

	threadsafe_fixed_queue ToSolveIds_; // for workers

	AnswerManager resultManager;

};

#endif // !_HEADERS_FABRIC_FABRIC_H_