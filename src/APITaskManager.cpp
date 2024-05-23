#include "../headers/APITaskManager.h"


MANAGER_INIT_ERROR APITaskManager::INIT(uint32_t answersCount, uint32_t tasksCount, uint32_t workersCount) {
	return this->fabric_.INIT(answersCount, tasksCount, workersCount);
}

bool APITaskManager::DISABLE() {
	return this->fabric_.DISABLE();
}

bool APITaskManager::StartUp() {
	return this->fabric_.FabricStartUp();
}

bool APITaskManager::ShutDown() {
	return this->fabric_.FabricShutDown();
}




bool APITaskManager::APINewTaskContainer(UNIC_KEY& key) {
	TASK_ID taskId;
	uint32_t hash;
	if (!this->fabric_.allocateSpaceForTask(taskId))
		return false;
	bool success = this->fabric_.getTaskHash(hash, taskId);
	ASSERT(success == true);

	key = ID_HASH_TO_KEY(taskId, hash);
	
	return true;

}

OpperationResult APITaskManager::APIStoreTask(const TASK_CONTAINER& taskContainer, UNIC_KEY key)
{
	TASK_ID taskId;
	OpRes success = ValidateKey(key, taskId);
	if (success == OpRes::DONE)
	{
		if (this->fabric_.storeTaskAndStartSolving(taskContainer, taskId) == false)
			return OpRes::NOT_DONE;
	}
	return success;
}

OpperationResult APITaskManager::APILoadTaskAnswer(TASK_ANSWER& answer, UNIC_KEY key)
{
	TASK_ID taskId;
	OpRes success = ValidateKey(key, taskId);
	if (success == OpRes::DONE)
	{
		AnswerContainer answerContainer;
		bool isSolwedResult = this->fabric_.getSolvedAnswerContainer(answerContainer, KEY_TO_ID(key));
		if (isSolwedResult == false)
			return OpRes::NOT_DONE;
		answer = answerContainer.mainResult;
	}
	return success;
}


OpperationResult APITaskManager::APIFreeTaskAnswer(UNIC_KEY key)
{
	TASK_ID taskId;
	OpRes success = ValidateKey(key, taskId);
	if (success == OpRes::DONE) {
		if (this->fabric_.freeAnswerContainer(taskId) == false)
			return OpRes::NOT_DONE;
	}
	return success;
}

OpperationResult APITaskManager::ValidateKey(UNIC_KEY key, TASK_ID & taskId)
{
	TASK_ID taskIdValidate = KEY_TO_ID(key);
	uint32_t  hash = KEY_TO_HASH(key);
	uint32_t  realHash = 0;
	bool success = this->fabric_.getTaskHash(realHash, taskIdValidate);
	if (!success) // do not exist object with this id
		return OpRes::NOT_FOUND;

	if (realHash != hash) // hash of object do not equal to getted hash. so it's invalid request.
		return OpRes::CAN_NOT_DO;

	taskId = taskIdValidate;

	return OpRes::DONE;
}
