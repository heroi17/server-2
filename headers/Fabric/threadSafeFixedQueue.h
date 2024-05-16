#pragma once
#ifndef _HEADERS_FABRIC_THREADSAFEQUEUE_H_
#define _HEADERS_FABRIC_THREADSAFEQUEUE_H_

#include "../GlobalInclude.h"


class threadsafe_fixed_queue
{
private:
	mutable std::mutex mut;
	std::queue<TASK_ID> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_fixed_queue();

	threadsafe_fixed_queue(threadsafe_fixed_queue const& other);

	void push(TASK_ID new_value);

	void wait_and_pop(TASK_ID& value);

	bool try_pop(TASK_ID& value);

	bool empty() const;

};

#endif // !_HEADERS_FABRIC_THREADSAFEQUEUE_H_