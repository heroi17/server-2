#include "../headers/TaskContainer/TaskContainer.h"
#include <random>
#include <iostream>

TASK_ANSWER TASK_CONTAINER::operator()() {
	TASK_ANSWER answer;
	this->func_(this->data_, answer);
	return answer;
}


TASK_CONTAINER::TASK_CONTAINER(void(*func)(const TASK_DATA&, TASK_ANSWER&), TASK_DATA data)
	: func_(func), data_(data) {}

