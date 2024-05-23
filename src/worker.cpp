#include "../headers/Fabric/worker.h"
#include <chrono>
bool Worker::SturtUp(TaskSolverFabric* myFabric)
{
	std::lock_guard lg(startUpMutex);
	if (isStartingUp_) return false;
	this->myFabric_ = myFabric;
	isStartingUp_ = true;
	this->myThread_ = std::thread(&Worker::WorkerLoop, this);
	return false;
}

bool Worker::ShutDown()
{
	std::lock_guard lg(startUpMutex);
	if (!isStartingUp_) return false;
	isStartingUp_ = false;

	// We should to know that our WorkerLoop not in wait_and_pop now.
	// We should put in queue ID: TERMINATE_TASK_ID and to know that our worker catch this TERMINATE_TASK_ID.
	// If there only one worker then we catch it, but if more then 1 second worker can catch it, stop working, and our worker be in deadLock.
	while (this->inWorkerLoop) {
		this->myFabric_->ToSolveIds_.push(TERMINATE_TASK_ID);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}


	this->myThread_.join();

	return false;
}

bool Worker::IsStartingUp() const
{
	return this->isStartingUp_;
}

Worker::~Worker()
{
	this->ShutDown();
}

void Worker::WorkerLoop()
{
	using namespace std::chrono;
	this->inWorkerLoop = true;
	while (this->isStartingUp_) {
		
		TASK_ID currentTaskId = 0;
		AnswerContainer* answerContainer = nullptr;
		TASK_CONTAINER* taskContainer = nullptr;

		this->myFabric_->ToSolveIds_.wait_and_pop(currentTaskId); // get task from queue
		if (currentTaskId == TERMINATE_TASK_ID) break;

		answerContainer = this->myFabric_->resultManager.getAnswerPtr(currentTaskId);
		taskContainer = this->myFabric_->resultManager.getTaskPtr(currentTaskId);
		ASSERT(answerContainer != nullptr); //it should be not nullptr
		ASSERT(taskContainer != nullptr); //it should be not nullptr
		ASSERT(answerContainer->stageOfTask == TASK_STATE::WAITING_IN_QUEUE_FOR_SOLWE); // only in this position we can solwe task
		answerContainer->stageOfTask = TASK_STATE::IS_SOLWEING_NOW;

		time_point StartTime{ system_clock::now() };
		answerContainer->mainResult = (*taskContainer)(); //Main work. we are parrallel programm for it.
		time_point EndTime{ system_clock::now() };
		answerContainer->timeOfWork = EndTime - StartTime;

		bool resultDelete = this->myFabric_->resultManager.taskDelete(currentTaskId);
		ASSERT(resultDelete == true);
		answerContainer->stageOfTask = TASK_STATE::DONE;

	}
	this->inWorkerLoop = false;
}

