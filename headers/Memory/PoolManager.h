#pragma once
#ifndef _HEADERS_POOLMANAGER_H_
#define _HEADERS_POOLMANAGER_H_

#include "../GlobalInclude.h"
#include <list>
//TODO: think about how to make object pool without templates, just by use (to_type*)




struct PoolManager {

	//return true if can allocate memory
	MANAGER_INIT_ERROR INIT(uint32_t capacity, uint32_t bytesSizeOfType); //


	//free data(if not init then return false);
	bool DISABLE();

	void* allocObj();

	//if this object exist, end allocated then return true + deallocate.
	bool freeObj(void* freeData);


	void* operator[](uint32_t index_); // return ptr on data in index

	bool getIndex(uint32_t& returnId, const void *const ptrObj_) const; // return true and change returnId

	uint32_t getCapacity() const;
	~PoolManager(); // We should to free dataPtr.
private:


	struct LinkedListNode {


		void disconnectAndSetAfter(LinkedListNode* afterNode);

		void setAfter(LinkedListNode* afterNode);

		LinkedListNode* next = nullptr;
		LinkedListNode* prev = nullptr;
		bool isAllocatedData = false;
	};

	void initLinkedList_();

	mutable std::mutex allocMutex_; //mutable for getters

	std::uint32_t capacity_=0; //max objects count in pull
	std::uint32_t BLOCK_SIZE=0; // size of block(LLN + object)
	std::uint32_t objectSize_=0;
	void* dataPtr = nullptr; //when isInit then it's not nullptr
	bool isInit = false;

	LinkedListNode HeadFree;
	LinkedListNode TailFree;
	LinkedListNode HeadAllocated;
	LinkedListNode TailAllocated;

};

#endif // !_HEADERS_POOLMANAGER_H_
