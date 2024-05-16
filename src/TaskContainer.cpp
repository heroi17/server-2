#include "../headers/TaskContainer/TaskContainer.h"
#include <random>
#include <iostream>
#define WANT_TO_SEE false
#if WANT_TO_SEE
#include "Windows.h"
#endif
TASK_ANSWER TASK_CONTAINER::operator()() {
	TASK_ANSWER retAnswer;
	size_t goodDots = 0;
	if (this->dots_ == 0) {
		retAnswer.result = 1;
		return retAnswer;
	}
	std::random_device rd;
	std::uniform_real_distribution <float> xDistr(this->rectangle_.xMin_, this->rectangle_.xMax_);
	std::uniform_real_distribution <float> yDistr(this->rectangle_.yMin_, this->rectangle_.yMax_);
#if WANT_TO_SEE
	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);
#endif
	for (size_t i = 0; i < this->dots_; i++) {
		
		float cur_x = xDistr(rd);
		float cur_y = yDistr(rd);
		bool toAdd = true;
		for (auto el : this->conditions_) {
			if (!(*el)(cur_x, cur_y)) {
				toAdd = false;
				break;
			}
		}
		if (toAdd)
		{
#if WANT_TO_SEE
			SetPixel(hdc, int(200 + cur_x*50), int(250 - cur_y * 50), RGB(255, 255, 255));
#endif
			goodDots++;
		}
	}
#if WANT_TO_SEE
	ReleaseDC(hwnd, hdc);
#endif
	retAnswer.result = ((double)goodDots) / this->dots_ * 
		(this->rectangle_.xMax_ - this->rectangle_.xMin_) * 
		(this->rectangle_.yMax_ - this->rectangle_.yMin_);

	return retAnswer;
}


TASK_CONTAINER::TASK_CONTAINER(Rect rectangle, size_t dots, taskVec conditions) : rectangle_(rectangle), dots_(dots), conditions_(conditions) {}

Rect::Rect(float xMin, float yMin, float xMax, float yMax) : xMin_(xMin), yMin_(yMin), xMax_(xMax), yMax_(yMax) {}
