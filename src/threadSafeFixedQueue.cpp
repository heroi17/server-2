#include "../headers/Fabric/threadSafeFixedQueue.h"

threadsafe_fixed_queue::threadsafe_fixed_queue() {}

threadsafe_fixed_queue::threadsafe_fixed_queue(threadsafe_fixed_queue const& other)
{
	std::lock_guard<std::mutex> lg(other.mut);
	data_queue = other.data_queue;
}

void threadsafe_fixed_queue::push(TASK_ID new_value)
{
	std::lock_guard<std::mutex> lk(mut);
	data_queue.push(new_value);
	data_cond.notify_one();
}

void threadsafe_fixed_queue::wait_and_pop(TASK_ID& value)
{
	std::unique_lock<std::mutex> lk(mut);
	data_cond.wait(lk, [this] {return !data_queue.empty(); });
	value = data_queue.front();
	data_queue.pop();
}

bool threadsafe_fixed_queue::try_pop(TASK_ID& value)
{
	std::lock_guard<std::mutex> lk(mut);
	if (data_queue.empty())
		return false;
	value = data_queue.front();
	data_queue.pop();
	return true;
}

bool threadsafe_fixed_queue::empty() const
{
	std::lock_guard<std::mutex> lk(mut);
	return data_queue.empty();
}




