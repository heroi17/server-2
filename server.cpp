// server 2.0.cpp: определяет точку входа для приложения.
//

#include "server.h"


//make TASK_DATA from input data
TASK_DATA makedata123(int a_, int b_) {
	TASK_DATA data;
	int* dataInt = (int*)data.task_data;
	dataInt[0] = a_;
	dataInt[1] = b_;
	return data;
}

//func thet use TASK_DATA and put answer into TASK_ANSWER
void func123(const TASK_DATA& Task_data, TASK_ANSWER& answerData) {
	int const* dataInt = (int const*)Task_data.task_data;
	((int*)answerData.answer_data)[0] = dataInt[0] + dataInt[1];
}


//extract the answer from TASK_ANSWER
int getAnswer(const TASK_ANSWER& answerData) {
	return ((int*)answerData.answer_data)[0];
}







int main() {
	using namespace std::chrono;
	APITaskManager myFabric;
	myFabric.INIT(100, 10, 5); // 10000 answer and 100 elem in queue.
	myFabric.StartUp();
	std::cout << "Working" << std::endl;

	UNIC_KEY myKey;
	std::vector<UNIC_KEY> keys;
	TASK_CONTAINER myTask { func123, makedata123(0, 0) };
	for (int i = 0; i< 100; i++) {
		
		if (myFabric.APINewTaskContainer(myKey))
		{
			myFabric.APIStoreTask({ func123, makedata123(rand()%10, rand()%10) }, myKey);
			keys.push_back(myKey);
		}
		std::this_thread::sleep_for(25ms);

	}
	TASK_ANSWER answ;
	while (myFabric.APILoadTaskAnswer(answ, keys[keys.size() - 2]) != OpperationResult::DONE) {
		std::cout << keys[keys.size() - 2] << " " << (myFabric.APILoadTaskAnswer(answ, keys[keys.size() - 1]) == OpperationResult::NOT_DONE) << std::endl;
		std::this_thread::sleep_for(100ms);
	}
	std::this_thread::sleep_for(100ms);
	for (auto el : keys)
	{
		TASK_ANSWER answ;

		myFabric.APILoadTaskAnswer(answ, el);

		std::cout << KEY_TO_ID(el) << ": " << getAnswer(answ) << std::endl;

	}
	std::cout << keys.size() << std::endl;


	myFabric.ShutDown();
	std::cout << "stop" << std::endl;
	myFabric.DISABLE();
	std::cout << "free" << std::endl;
	return 0;
}