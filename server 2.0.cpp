// server 2.0.cpp: определяет точку входа для приложения.
//

#include "server 2.0.h"
#define PI 3.14159265358979
#include <functional>
#include <numeric>




// you can change area and add some condition in line 33 to taskCon;
Rect AREA = Rect(0, 0, 2, 2);

bool con1(float x, float y) {
	return x*x+y*y < 4;
}

bool con2(float x, float y) { // you can use many condition 
	return x * x + y * y > 3;
}

bool con3(float x, float y) {
	return pow((pow(y-2, 6) + x-4 + y), 2) < 2;
}

int testForDots(size_t dots, TaskSolverFabric& myFabric, size_t ITERATION) {
	if (ITERATION == 0) ITERATION = 1;
	std::vector<TASK_ID> ids;
	double ANSWER = 0;
	decltype(AnswerContainer::timeOfWork) TIME_DURATION { 0 };
	std::vector<double> SUM_ERROR;

	for (size_t i = 0; i < ITERATION; i++) {
		TASK_ID myTask;
		TASK_CONTAINER taskCon = TASK_CONTAINER(AREA, dots, { con1 }); // <<here can be used two or more condition.
		if (myFabric.allocateSpaceForTask(myTask)) {
			myFabric.storeTaskAndStartSolving(taskCon, myTask);
			ids.push_back(myTask);
		}

	}
	while (ids.size() != 0) {
		ids.erase(std::remove_if(ids.begin(), ids.end(), [&myFabric, &SUM_ERROR, &ANSWER, &TIME_DURATION](const TASK_ID& id)-> bool
			{
				TASK_STATE state;
				myFabric.getTaskState(state, id);
				if (state == TASK_STATE::DONE) {
					AnswerContainer answer;
					myFabric.getSolvedAnswerContainer(answer, id);
					myFabric.freeAnswerContainer(id);
					SUM_ERROR.push_back(abs(answer.mainResult.result - PI));

					ANSWER = answer.mainResult.result;
					TIME_DURATION = answer.timeOfWork;
					return true;
				}
				return false;
			}), ids.end());

		_sleep(50);
	}
	double goodTest = 0;
	int tests = 0;
	for (double E = 0.1; E > 1E-5; E *= 0.3) {
		double chance = 0;
		for (auto error : SUM_ERROR) {
			if (error / 4 >= E) chance += 1;
		}
		chance /= SUM_ERROR.size();
		if (chance <= (4. - PI) / (dots * E * E * 4.)) goodTest++;

		tests++;
	}

	double SUM_SUM_ERROR = std::accumulate(SUM_ERROR.begin(), SUM_ERROR.end(), 0.);
	std::cout << "time/answer/error/dots/formulaWork \t" << duration_cast<std::chrono::milliseconds>(TIME_DURATION) << "\t" << ANSWER << "\t" << (SUM_SUM_ERROR / ITERATION) << "\t" << "2 **" << log2(dots) << "\t" << (goodTest / tests) << std::endl;
	
	//We should check the formula: p(error/4 >= e) <= (4-pi) / (dots * e * e * 16)
	
	
	
	
	
	return 0;
}


int main() {
	std::cout << "calculating pi walue" << std::endl;
	size_t iteration = 10;
	TaskSolverFabric myFabric;
	myFabric.INIT(iteration, iteration, 5);
	myFabric.FabricStartUp();
	for (int i = 4; i < 25; i++) {
		testForDots(pow(2, i), myFabric, iteration);
	}
	return 0;
}