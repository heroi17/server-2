// server 2.0.cpp: определяет точку входа для приложения.
//

#include "server.h"

int main() {
	using namespace std::chrono;
	APITaskManager myFabric;
	myFabric.INIT(100, 10, 5); // 10000 answer and 100 elem in queue.
	myFabric.StartUp();
	std::cout << "Working" << std::endl;

	UNIC_KEY myKey;
	std::vector<UNIC_KEY> keys;
	TASK_CONTAINER myTask {100, 2};
	for (int i = 0; i< 100; i++) {
		
		if (myFabric.APINewTaskContainer(myKey))
		{
			myFabric.APIStoreTask({rand()%10, rand()%10}, myKey);
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

		std::cout << KEY_TO_ID(el) << ": " << answ.result << std::endl;

	}
	std::cout << keys.size() << std::endl;


	myFabric.ShutDown();
	std::cout << "stop" << std::endl;
	myFabric.DISABLE();
	std::cout << "free" << std::endl;
	return 0;
}