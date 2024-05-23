//
// 
//           this class can be used only in TaskSolverFabric
// 
// 
// 
// 
//


#pragma once
#ifndef _HEADERS_FABRIC_ANSWER_MANAGER_H_
#define _HEADERS_FABRIC_ANSWER_MANAGER_H_


#include "../GlobalInclude.h"
#include "../Memory/PoolManager.h"
class TaskSolverFabric;

//будет выделять месть для данных и освобождать


class AnswerManager {
	friend class TaskSolverFabric;
public:
	MANAGER_INIT_ERROR INIT(uint32_t answersSize, uint32_t  taskSize);

	bool DISABLE();

	// try to allocate, return true if allocated
	bool tryToAllocateNewID(TASK_ID&);

	// if task_container with this ID is exist, then delete and return true
	bool taskDelete(TASK_ID);

	// if answer_container with this ID is exist, then delete and return true
	// (also if task with this id is exist, then it'l be deleted)
	bool answerDelete(TASK_ID&);

	//Return nullptr if Invalid Task_ID
	TASK_CONTAINER* getTaskPtr(TASK_ID);

	AnswerContainer* getAnswerPtr(TASK_ID);

private:
	PoolManager taskContainerPool_; // allocates spaces for data for task
	PoolManager answersContainers_;
	bool isInit = false;
	std::mutex allocationMutex;
};

#endif // !_HEADERS_FABRIC_ANSWER_MANAGER_H_