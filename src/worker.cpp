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

	// wait for end my thread.
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
	while (this->isStartingUp_) {
		
		TASK_ID currentTaskId = 0;
		AnswerContainer* answerContainer = nullptr;
		TASK_CONTAINER* taskContainer = nullptr;
		this->myFabric_->ToSolveIds_.wait_and_pop(currentTaskId); // get task from queue
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
}

