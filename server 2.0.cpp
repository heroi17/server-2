// server 2.0.cpp: определяет точку входа для приложения.
//

#include "server 2.0.h"

int main() {
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
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	}


	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	for (auto el : keys)
	{
		TASK_ANSWER answ;

		myFabric.APILoadTaskAnswer(answ, el);

		std::cout << el << ": " << answ.result << std::endl;

	}
	std::cout << keys.size() << std::endl;


	myFabric.ShutDown();
	std::cout << "stop" << std::endl;
	myFabric.DISABLE();
	std::cout << "free" << std::endl;
	return 0;
}