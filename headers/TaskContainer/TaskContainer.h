#pragma once
#ifndef _HEADERS_TASKCONTAINER_TASKCONTAINER_H_
#define _HEADERS_TASKCONTAINER_TASKCONTAINER_H_
#include <vector>
//This module is just an Example of how to use program
//here we build containers after that we call TASK_CONTAINER operator()  and get result.
struct Rect {
	Rect(float xMin = 0, float yMin = 0, float xMax = 0, float yMax = 0);
	float xMin_;
	float yMin_;
	float xMax_;
	float yMax_;

};

struct TASK_ANSWER {
	double result;
};

struct TASK_CONTAINER {
	using taskVec = std::vector <bool(*)(float, float)>;

	TASK_CONTAINER(Rect rectangle = Rect(), size_t dots = 100000, taskVec conditions = taskVec());

	TASK_ANSWER operator()();

	Rect rectangle_;
	size_t dots_;
	taskVec conditions_;
};




#endif // !_HEADERS_TASKCONTAINER_TASKCONTAINER_H_