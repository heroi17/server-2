#define PI 3.14159265358979
#include <functional>
#include <numeric>
#include "AADSMonteCarlo.h"
#include <random>
#define WANT_TO_SEE false
#if WANT_TO_SEE
#include "Windows.h"
#endif

// -------------PREPARE STRUCTURE FOR MONTE CARLO------------
struct Rect {
	Rect(float xMin = 0, float yMin = 0, float xMax = 0, float yMax = 0) {
		xMin_ = xMin;
		yMin_ = yMin;
		xMax_ = xMax;
		yMax_ = yMax;
	};
	float xMin_;
	float yMin_;
	float xMax_;
	float yMax_;

};

struct helperMonteCarlo {
	Rect area;
	uint32_t dots;
	bool (*condition)(float, float);
};

TASK_DATA makeDataForMonteCarlo(Rect area, uint32_t dots, bool (*condition)(float, float)) {
	ASSERT(sizeof(helperMonteCarlo) <= TASK_DATA_SIZE);
	TASK_DATA data;
	((helperMonteCarlo*)(void*)data.task_data)[0] = { area, dots, condition };
	return data;
}

void funcForMonteCarlo(const TASK_DATA& Task_data, TASK_ANSWER& answerData) {
	helperMonteCarlo realData = ((helperMonteCarlo*)Task_data.task_data)[0];
	size_t goodDots = 0;
	if (realData.dots == 0) {
		((int*)answerData.answer_data)[0] = 1;
		return void();
	}
	std::random_device rd;
	std::uniform_real_distribution <float> xDistr(realData.area.xMin_, realData.area.xMax_);
	std::uniform_real_distribution <float> yDistr(realData.area.yMin_, realData.area.yMax_);
#if WANT_TO_SEE
	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);
#endif
	for (size_t i = 0; i < realData.dots; i++) {
		float cur_x = xDistr(rd);
		float cur_y = yDistr(rd);
		bool toAdd = true;
		if (!(*realData.condition)(cur_x, cur_y)) {
			toAdd = false;
		}
		
		if (toAdd)
		{
#if WANT_TO_SEE
			SetPixel(hdc, int(200 + cur_x * 50), int(250 - cur_y * 50), RGB(255, 255, 255));
#endif
			goodDots++;
		}
	}
#if WANT_TO_SEE
	ReleaseDC(hwnd, hdc);
#endif

	((float*)answerData.answer_data)[0] = (((double)goodDots) / realData.dots *
		(realData.area.xMax_ - realData.area.xMin_) *
		(realData.area.yMax_ - realData.area.yMin_));
	return void();
	
}

float getAnswerForMonteCarlo(const TASK_ANSWER& answerData) {
	//std::cout << "real Amswer: " << ((float*)answerData.answer_data)[0] << std::endl;
	return ((float*)answerData.answer_data)[0];
}







// -------------MAIN PART OF MONTE CARLO------------

Rect AREA = Rect(0, 0, 2, 2);

bool con1(float x, float y) {
	return x * x + y * y < 4;
}


int testForDots(size_t dots, TaskSolverFabric& myFabric, size_t ITERATION) {
	if (ITERATION == 0) ITERATION = 1;
	std::vector<TASK_ID> ids;
	double ANSWER = 0;
	decltype(AnswerContainer::timeOfWork) TIME_DURATION{ 0 };
	std::vector<double> SUM_ERROR;

	for (size_t i = 0; i < ITERATION; i++) {
		TASK_ID myTask;
		auto dataForTask = makeDataForMonteCarlo(AREA, dots, con1);
		TASK_CONTAINER taskCon = TASK_CONTAINER(funcForMonteCarlo, dataForTask);
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
					SUM_ERROR.push_back(abs(getAnswerForMonteCarlo(answer.mainResult) - PI));

					ANSWER = getAnswerForMonteCarlo(answer.mainResult);
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
	std::cout << "time/answer/error/dots/formulaWork \t" 
		<< duration_cast<std::chrono::milliseconds>(TIME_DURATION) << "\t"
		<< ANSWER << "\t" << (SUM_SUM_ERROR / ITERATION) << "\t"
		<< "2 **" << log2(dots) << "\t"
		<< (goodTest / tests) << std::endl;

	//We should check the formula: p(error/4 >= e) <= (4-pi) / (dots * e * e * 16)





	return 0;
}


void monteCarloTest() {
	std::cout << "calculating pi walue" << std::endl;
	size_t iteration = 10;
	TaskSolverFabric myFabric;
	myFabric.INIT(iteration, iteration, 5);
	myFabric.FabricStartUp();
	for (int i = 4; i < 25; i++) {
		testForDots(pow(2, i), myFabric, iteration);
	}
	return void();
}