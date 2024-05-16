#pragma once
#ifndef _HEADERS_TASKCONTAINER_TASKCONTAINER_H_
#define _HEADERS_TASKCONTAINER_TASKCONTAINER_H_
#include <vector>
//This module is just an Example of how to use program
//here we build containers after that we call TASK_CONTAINER operator()  and get result.
struct TASK_ANSWER {
	double result;
};

struct TASK_CONTAINER {
	using taskVec = std::vector <bool(*)(float, float)>;

	TASK_CONTAINER(int a, int b);

	TASK_ANSWER operator()();

	int a_;
	int b_;
};




#endif // !_HEADERS_TASKCONTAINER_TASKCONTAINER_H_