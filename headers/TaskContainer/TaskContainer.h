#pragma once
#ifndef _HEADERS_TASKCONTAINER_TASKCONTAINER_H_
#define _HEADERS_TASKCONTAINER_TASKCONTAINER_H_
#include <vector>
//This module is just an Example of how to use program
//here we build containers after that we call TASK_CONTAINER operator()  and get result.
#define ANSWER_DATA_SIZE 128
#define TASK_DATA_SIZE 128


struct TASK_ANSWER {
	uint8_t answer_data[ANSWER_DATA_SIZE];
};

struct TASK_DATA {
	uint8_t task_data[TASK_DATA_SIZE];
};
struct TASK_CONTAINER {
	
	TASK_DATA data_;

	void (*func_)(const TASK_DATA&, TASK_ANSWER&);

	TASK_CONTAINER(void(*func)(const TASK_DATA&, TASK_ANSWER&), TASK_DATA data);

	TASK_ANSWER operator()();
};




#endif // !_HEADERS_TASKCONTAINER_TASKCONTAINER_H_