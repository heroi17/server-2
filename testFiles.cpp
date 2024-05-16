#include "testFiles.h"
using namespace std;

struct helper {
	static int index_counter_helper;
	int index;
	int haha;
	size_t rubric;


	void init(int nhaha, size_t nrubric) {
		index = index_counter_helper++;
		haha = nhaha;
		rubric = nrubric;
	}

	void operator()() const {
		if (this != nullptr)
			std::cout << "id:" << index << " - " << haha << "\t";
		else
			std::cout << "nullptr" << "\t";
	}

};
int helper::index_counter_helper = 0;

int test1() {
	PoolManager myPool;
	myPool.INIT(10, sizeof(helper));
	std::vector<helper*> all;
	for (int i = 0; i < 10; i++) {
		all.push_back((helper*)myPool.allocObj());
	}
	std::for_each(all.begin(), all.end(), [](helper* el) {if (el) el->init(10, 11); });
	std::for_each(all.begin(), all.end(), [&myPool](helper* el) {if (el) myPool.freeObj(el); });
	all.clear();
	for (int i = 0; i < 10; i++) {
		all.push_back((helper*)myPool.allocObj());
	}
	std::for_each(all.begin(), all.end(), [&myPool](helper* el) {
		size_t answer = 9999;
		myPool.getIndex(answer, (void*)el);
		std::cout << answer << "\t";
		});
	system("pause");
	return 0;
}


int test2() {
	AnswerManager am;
	TASK_ID task_1;
	am.INIT(111, 1);
	std::cout << "try alloc  " << am.tryToAllocateNewID(task_1) << std::endl;
	std::cout << "try alloc  " << am.tryToAllocateNewID(task_1) << std::endl;
	std::cout << "try alloc  " << am.tryToAllocateNewID(task_1) << std::endl;
	std::cout << "try delete " << am.answerDelete(task_1) << std::endl;
	std::cout << "try alloc  " << am.tryToAllocateNewID(task_1) << std::endl;
	std::cout << "try alloc  " << am.tryToAllocateNewID(task_1) << std::endl;
	std::cout << "try delete " << am.answerDelete(task_1) << std::endl;
	std::cout << "try alloc  " << am.tryToAllocateNewID(task_1) << std::endl;
	return 0;
}

bool condition1(float x, float y) {
	//return true;
	return x + 3 * y > 3.5;
}
bool condition2(float x, float y) {
	//return true;
	return y < powf(2, -x * x);
}

int test3() {
	TaskSolverFabric myFabric;
	myFabric.INIT(100, 100, std::thread::hardware_concurrency() - 2);
	myFabric.FabricStartUp();
	std::vector<TASK_ID> ids;
	for (int i = 1; i < 28; i++) {
		TASK_ID myTask;
		TASK_CONTAINER taskCon = TASK_CONTAINER(Rect(0, 0, 3, 1), pow(2, i), { condition2 });
		if (myFabric.allocateSpaceForTask(myTask)) {
			myFabric.storeTaskAndStartSolving(taskCon, myTask);
			ids.push_back(myTask);
		}

	}
	while (ids.size() != 0) {
		TASK_STATE curState;
		int done = 0;
		int wait_for_load = 0;
		int wait_in_queue = 0;
		int is_solving = 0;
		AnswerContainer answer;
		for (auto el : ids) {
			myFabric.getTaskState(curState, el);
			switch (curState)
			{
			case TASK_STATE::DONE:
			{
				done++;
				myFabric.getSolvedAnswerContainer(answer, el);
				myFabric.freeAnswerContainer(el);
				std::cout << answer.mainResult.result << std::endl;
				break;

			}
			case TASK_STATE::WAITING_FOR_LOAD_TASK_CONTAINER:
				wait_for_load++;
				break;
			case TASK_STATE::WAITING_IN_QUEUE_FOR_SOLWE:
				wait_in_queue++;
				break;
			case TASK_STATE::IS_SOLWEING_NOW:
				is_solving++;
				break;
			default:
				break;
			}
		}
		ids.erase(std::remove_if(ids.begin(), ids.end(), [&](const TASK_ID& id)-> bool
			{
				TASK_STATE state;
				myFabric.getTaskState(state, id);
				return state == TASK_STATE::DONE;
			}), ids.end());
		std::cout << "is done    " << done << std::endl;
		std::cout << "no task    " << wait_for_load << std::endl;
		std::cout << "in queue   " << wait_in_queue << std::endl;
		std::cout << "is solving " << is_solving << std::endl << std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		_sleep(5000);
		//system("cls");
	}




	//if (myFabric.getTaskState(curState, myTask)) {
	//	switch (curState)
	//	{
	//	case TASK_STATE::DONE:
	//		std::cout << "is done" << std::endl;
	//		break;
	//	case TASK_STATE::WAITING_FOR_LOAD_TASK_CONTAINER:
	//		std::cout << "no task" << std::endl;
	//		break;
	//	case TASK_STATE::WAITING_IN_QUEUE_FOR_SOLWE:
	//		std::cout << "in queue" << std::endl;
	//		break;
	//	case TASK_STATE::IS_SOLWEING_NOW:
	//		std::cout << "is solving" << std::endl;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	return 0;
}

bool testOutput() {
	std::cout << "Is called" << std::endl;
	return false;
}