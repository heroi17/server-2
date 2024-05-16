#include "../headers/Fabric/AnswerManager.h"

MANAGER_INIT_ERROR AnswerManager::INIT(std::size_t answersSize, std::size_t taskSize)
{
	std::lock_guard lg(allocationMutex);
	using MIE = MANAGER_INIT_ERROR;
	if (this->isInit) return MIE::OLREADY_INIT;
	if (answersSize == 0 || taskSize == 0) return MIE::INVALID_INIT_DATA;

	auto taskError = this->taskContainerPool_.INIT(taskSize, sizeof(TASK_CONTAINER));
	auto answerError = this->answersContainers_.INIT(answersSize, sizeof(AnswerContainer));

	if (taskError == MIE::INIT_OKAY &&
		answerError == MIE::INIT_OKAY)
	{
		this->isInit = true;
		return MIE::INIT_OKAY;
	}
	else { // it can be only if one of errors is CAN_NOT_ALLOCATE_MEMORY
		this->taskContainerPool_.DISABLE();
		this->answersContainers_.DISABLE();
		return MIE::CAN_NOT_ALLOCATE_MEMORY;
	}
}

bool AnswerManager::DISABLE()
{
	std::lock_guard lg(allocationMutex);
	if (!this->isInit)
		return false;
	this->isInit = false;
	this->taskContainerPool_.DISABLE();
	this->answersContainers_.DISABLE();
	return true;
}

bool AnswerManager::tryToAllocateNewID(TASK_ID& newId) //this id is index in answerPool
{
	AnswerContainer* newAnswer;
	TASK_CONTAINER* newTaskContainer;
	{
		std::lock_guard lg(allocationMutex);
		if (!this->isInit) return false;
		newAnswer = (AnswerContainer*)this->answersContainers_.allocObj();
		newTaskContainer = (TASK_CONTAINER*)this->taskContainerPool_.allocObj();
		if ((newAnswer == nullptr) or (newTaskContainer == nullptr)) { // then just free.
			this->answersContainers_.freeObj(newAnswer);
			this->taskContainerPool_.freeObj(newTaskContainer);
			return false;
		}
	}
	newAnswer->taskContainer = newTaskContainer; // connect task container to solwe container
	this->answersContainers_.getIndex(newId, (void*)newAnswer);
	return true;
}

bool AnswerManager::taskDelete(TASK_ID toDeleteId)
{
	auto* AC = getAnswerPtr(toDeleteId);
	bool answer = this->taskContainerPool_.freeObj(AC->taskContainer);
	AC->taskContainer = nullptr;
	return answer;
}

bool AnswerManager::answerDelete(TASK_ID& toDeleteId)
{
	taskDelete(toDeleteId);
	return this->answersContainers_.freeObj(getAnswerPtr(toDeleteId));
}

TASK_CONTAINER* AnswerManager::getTaskPtr(TASK_ID task_id)
{
	AnswerContainer* ans_Container = (AnswerContainer*)this->answersContainers_[task_id];
	if (ans_Container == nullptr) return nullptr;
	return ans_Container->taskContainer;
}

AnswerContainer* AnswerManager::getAnswerPtr(TASK_ID task_id)
{
	return (AnswerContainer*)this->answersContainers_[task_id];
}
