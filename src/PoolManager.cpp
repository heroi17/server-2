#include "../headers/Memory/PoolManager.h"

#define adress(ptr, i, blockSize) ((BYTE*)(this->dataPtr) + (i) * (blockSize))
#define bytesBetween(ptr1, ptr2) ((BYTE*)(ptr1) - (BYTE*)(ptr2))



MANAGER_INIT_ERROR PoolManager::INIT(std::size_t capacity, std::size_t bytesSizeOfType)
{
	if (capacity == 0 || bytesSizeOfType == 0) return MANAGER_INIT_ERROR::INVALID_INIT_DATA;
	std::lock_guard lg(allocMutex_);
	std::size_t BLOCK_SIZE_NEW = sizeof(LinkedListNode) + bytesSizeOfType;
	if (this->isInit) return MANAGER_INIT_ERROR::OLREADY_INIT;
	this->dataPtr = calloc(BLOCK_SIZE_NEW, capacity);
	if (!this->dataPtr) return MANAGER_INIT_ERROR::CAN_NOT_ALLOCATE_MEMORY;
	BLOCK_SIZE = BLOCK_SIZE_NEW;
	//init free list;
	this->capacity_ = capacity;
	this->objectSize_ = bytesSizeOfType;

	initLinkedList_();

	this->isInit = true;
	return MANAGER_INIT_ERROR::INIT_OKAY;
}

bool PoolManager::DISABLE()
{
	std::lock_guard lg(allocMutex_);
	if (!this->isInit) return false;
	free(this->dataPtr);
	this->capacity_ = 0;
	this->dataPtr = nullptr;
	this->objectSize_ = 0;
	this->isInit = false;
	return false;
}

void* PoolManager::allocObj()
{
	std::lock_guard lg(allocMutex_);
	if (HeadFree.next == &TailFree) 
		return nullptr;

	PoolManager::LinkedListNode* toAllocateNode = HeadFree.next; // (uint8_t*)this->HeadFree + sizeof(PoolManager::LinkedListNode);

	toAllocateNode->disconnectAndSetAfter(&HeadAllocated);
	toAllocateNode->isAllocatedData = true;


	return (void*)(toAllocateNode+1);
}

bool PoolManager::freeObj(void* freeData)
{
	std::lock_guard lg(allocMutex_);

	LinkedListNode* toDeleteNode = (((LinkedListNode*)freeData) - 1);
	if (toDeleteNode < this->dataPtr) return false;
	if (bytesBetween(toDeleteNode, this->dataPtr) % BLOCK_SIZE != 0) return false; // we should delete only correct data;
	size_t index = bytesBetween(toDeleteNode, this->dataPtr) / BLOCK_SIZE;
	if (index > capacity_ - 1) return false;

	if (!toDeleteNode->isAllocatedData) return false;

	toDeleteNode->disconnectAndSetAfter(&HeadFree);
	toDeleteNode->isAllocatedData = false;
	freeData = nullptr;
	return true;
}

void* PoolManager::operator[](std::size_t index_)
{
	if (index_ >= capacity_) return nullptr;
	return (void*)(((LinkedListNode*)adress(this->dataPtr, index_, BLOCK_SIZE)) + 1);
}

bool PoolManager::getIndex(size_t& returnId, const void *const ptrObj_) const
{
	std::lock_guard lg(allocMutex_);
	LinkedListNode* nodePtr = ((LinkedListNode*)ptrObj_ - 1); //maybe it's not correct data there
	if (nodePtr < this->dataPtr) return false; //check for correct ptr
	if (bytesBetween(nodePtr, this->dataPtr) % BLOCK_SIZE != 0) return false; // we should delete only correct data;
	size_t index = bytesBetween(nodePtr, this->dataPtr) / BLOCK_SIZE;
	returnId = index;
	return true;
}

std::size_t PoolManager::getCapacity() const
{
	return this->capacity_;
}

PoolManager::~PoolManager()
{
	this->DISABLE();
}

void PoolManager::initLinkedList_()
{
	using LLNP = PoolManager::LinkedListNode*;
	using LLN = PoolManager::LinkedListNode;

	this->HeadAllocated.next = &this->TailAllocated;
	this->TailAllocated.prev = &this->HeadAllocated;

	std::size_t BLOCK_SIZE = sizeof(LLN) + objectSize_;
	// First elem init.

	this->HeadFree.prev = nullptr;
	this->HeadFree.next = &this->TailFree;
	this->TailFree.prev = &this->HeadFree;
	this->TailFree.next = nullptr;
	LLNP ptr;
	LLNP lastPtr = &this->HeadFree;
	for (size_t i = 0; i < capacity_; i++) {
		ptr = (LLNP)adress(dataPtr, i, BLOCK_SIZE);
		ptr->setAfter(lastPtr);
		lastPtr = ptr;
	}
}


void PoolManager::LinkedListNode::disconnectAndSetAfter(LinkedListNode* afterNode)
{
	//std::cout << " BLOCK_SIZE" << (int)bytesBetween(this, this->next) << std::endl;
	//std::cout << " " << this << " " << this->next << std::endl;
	this->next->prev = this->prev; //disconnect old
	this->prev->next = this->next;

	setAfter(afterNode);
}

void PoolManager::LinkedListNode::setAfter(LinkedListNode* afterNode)
{
	this->next = afterNode->next; //connect this to new
	this->prev = afterNode;

	afterNode->next->prev = this; //connect new to this
	afterNode->next = this;
}
