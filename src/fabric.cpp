#include "../headers/Fabric/fabric.h"
#include <functional>

MANAGER_INIT_ERROR TaskSolverFabric::INIT(size_t answersCount, size_t tasksCount, size_t workersCount)
{
	std::lock_guard lg(accessMutex);
	if (isInit) return MANAGER_INIT_ERROR::OLREADY_INIT;
	auto error = this->resultManager.INIT(answersCount, tasksCount);
	if (error != MANAGER_INIT_ERROR::INIT_OKAY)
		return error;


	workers_.clear();
	for (size_t i = 0; i < workersCount; i++) {
		workers_.emplace_back();
	}

	isInit = true;
	return MANAGER_INIT_ERROR::INIT_OKAY;
}

bool TaskSolverFabric::DISABLE()
{
	std::lock_guard lg(accessMutex);
	if (!isInit) return false; //already not init
	FabricShutDown();
	workers_.clear();
	this->resultManager.DISABLE();
	isInit = false;
	return true;

}

bool TaskSolverFabric::FabricStartUp()
{
	std::lock_guard lg(accessMutex);
	if (!isInit) return false;
	if (isStartingUp) return false;
	std::for_each(workers_.begin(), workers_.end(), [this](Worker& el) {el.SturtUp(this); });
	
	this->isStartingUp = true;

	//TODO: add start managerTime.

	return true;
}

bool TaskSolverFabric::FabricShutDown()
{
	std::lock_guard lg(accessMutex);
	if (!isInit) return false;
	if (!isStartingUp) return false;

	
	std::for_each(workers_.begin(), workers_.end(), std::mem_fn(&Worker::ShutDown));

	isStartingUp = false;

	return true;

}

bool TaskSolverFabric::allocateSpaceForTask(TASK_ID& newId)
{
	TASK_ID newIdIn_ = 0;
	bool isAllocated = this->resultManager.tryToAllocateNewID(newIdIn_);
	if (isAllocated)
	{
		//it always should exist!!
		auto* answerPtr = this->resultManager.getAnswerPtr(newIdIn_);
		
		ASSERT(answerPtr != nullptr); //always should be not nullptr!
		//may be add layter (checking for old task)
		//answerPtr->timeWhenDelete = std::chrono::system_clock::now() + std::chrono::days(5);
		answerPtr->stageOfTask = TASK_STATE::WAITING_FOR_LOAD_TASK_CONTAINER;
		answerPtr->HASH = ((size_t)rand()); //hash should be 32 bits(first)
		newId = newIdIn_; 
	}
	return isAllocated;
}

bool TaskSolverFabric::storeTaskAndStartSolving(const TASK_CONTAINER& newContainer, TASK_ID taskId)
{
	std::lock_guard lg(accessMutex);
	auto* answerPtr = this->resultManager.getAnswerPtr(taskId);
	auto* taskPtr = this->resultManager.getTaskPtr(taskId);
	if (answerPtr == nullptr) return false;
	if (answerPtr->stageOfTask != TASK_STATE::WAITING_FOR_LOAD_TASK_CONTAINER) return false;
	(*this->resultManager.getTaskPtr(taskId)) = newContainer; //put container there.
	answerPtr->stageOfTask = TASK_STATE::WAITING_IN_QUEUE_FOR_SOLWE;
	ToSolveIds_.push(taskId); //if id in ToSolveIds_ then workers can get id and start solwe it
	return true;
}

bool TaskSolverFabric::getSolvedAnswerContainer(AnswerContainer& retAnswer, TASK_ID taskId)
{
	std::lock_guard lg(accessMutex); 
	auto* answer_ptr = this->resultManager.getAnswerPtr(taskId);
	if (answer_ptr == nullptr) return false;
	if (answer_ptr->stageOfTask == TASK_STATE::DONE)
	{
		retAnswer = *answer_ptr; // copy to return
		//i think it's better for security
		//if we will free it and save then someone who use fabric will see internal ptr. and i don't really like it.
		return true;
	}
	return false;

}

bool TaskSolverFabric::freeAnswerContainer(TASK_ID taskId)
{
	std::lock_guard lg(accessMutex);
	auto* toDeleteAnswerPtr = this->resultManager.getAnswerPtr(taskId);
	if (toDeleteAnswerPtr == nullptr) return false;
	if (toDeleteAnswerPtr->stageOfTask == TASK_STATE::IS_SOLWEING_NOW or
		toDeleteAnswerPtr->stageOfTask == TASK_STATE::WAITING_IN_QUEUE_FOR_SOLWE) return false; //we break internal work if delete in current state!
	bool deleteResult = this->resultManager.answerDelete(taskId);
	return true;
}

bool TaskSolverFabric::getTaskState(TASK_STATE& returnTaskState, TASK_ID taskId)
{
	std::lock_guard lg(accessMutex);
	auto* answerPtr = this->resultManager.getAnswerPtr(taskId);
	if (answerPtr == nullptr)
		return false;

	returnTaskState = answerPtr->stageOfTask;

	return true;
}

bool TaskSolverFabric::getTaskHash(size_t& hash, TASK_ID taskId)
{
	std::lock_guard lg(accessMutex);
	auto* answerPtr = this->resultManager.getAnswerPtr(taskId);
	if (answerPtr == nullptr)
		return false;

	hash = answerPtr->HASH;

	return true;
}

TaskSolverFabric::~TaskSolverFabric()
{
	this->DISABLE();
}


