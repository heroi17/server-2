//
//         Made by Markov Mikhail 08.05.2024
//       University project - Server(For study)
// 
// 
// 
//       
// 
//                |KLIENT|
//                   ~
//   			     ~
//                   ~
//           |SOCET part of server|
//                   |
//                   |
//             |TASKMANAGERAPI|
//                   |
//         ._________|________________________________________.
//         |.______________.<-->|task id's queue|<--.		  |
//         ||Answer manager|                        V		  |
//         ||______________|<->|WORKER| |WORKER|...... |..| | |
//         |__________________________________________________|
// 
// 
/////////////////////////////////////////////






#pragma once
#ifndef _HEADERS_GLOBALINCLUDE_H_
#define _HEADERS_GLOBALINCLUDE_H_

#include <iostream>
#include <chrono>
#include <mutex>
#include <queue>
#include "ASSERT.h"
#include "TaskContainer/TaskContainer.h"


typedef std::size_t TASK_ID; //this id is index in answerPool in AnswerManager
typedef std::size_t UNIC_KEY;
typedef std::uint8_t BYTE;



enum class TASK_STATE {
	DONE, // can get answer to user/ or / can delete data when time is left
	WAITING_FOR_LOAD_TASK_CONTAINER, // not in task queue, wait when user load task in task_container / or / can delete data when time is left
	WAITING_IN_QUEUE_FOR_SOLWE, // can't be deleted, wait for solve
	IS_SOLWEING_NOW // can't be deleted

};

enum class MANAGER_INIT_ERROR {
	OLREADY_INIT,
	INIT_OKAY,
	CAN_NOT_ALLOCATE_MEMORY,
	INVALID_INIT_DATA
};


//It should be no default values, because it be used in calloc memory.
struct AnswerContainer {
	friend class AnswerManager;
private:
	//it can be changed only in AnswerManager because he managed taskContainer also
	TASK_CONTAINER* taskContainer = nullptr; // Pointer on allocated data, where task were will be located
public:
	std::size_t HASH; // uniq number for people who try to get success to other's data
	TASK_STATE stageOfTask;
	TASK_ANSWER mainResult; // most necessary part!! it'll return to user
	std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> timeOfWork;
	std::chrono::time_point<std::chrono::system_clock> timeWhenDelete;

	TASK_CONTAINER* getTaskContainer();

}; // when delete, it's free taskContainer




enum class NewTaskContainerError {
	NO_FREE_ANSWER_PLACE,
	NO_FREE_TASK_CONTAINER_PLACE,
	OK
};

enum class OpperationResult {
	DONE,
	NOT_FOUND,
	NOT_DONE,
	TRUE = DONE,
	DONT_KNOW = NOT_FOUND,
	FALSE = NOT_DONE
};
#endif // !_HEADERS_GLOBALINCLUDE_H_
