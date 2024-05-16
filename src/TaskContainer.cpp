#include "../headers/TaskContainer/TaskContainer.h"
#include <random>
#include <iostream>

TASK_ANSWER TASK_CONTAINER::operator()() {
	TASK_ANSWER answer;
	answer.result = a_ + b_;
	return answer;
}


TASK_CONTAINER::TASK_CONTAINER(int a, int b) : a_(a), b_(b) {}

