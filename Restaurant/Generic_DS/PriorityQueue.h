#ifndef PriorityQueue_h
#define PriorityQueue_h

#include "LinkedSortedList.h"

template <typename T>
class PriorityQueue
{
private:
	LinkedSortedList<T>* sListPtr;
public:
	PriorityQueue();
	PriorityQueue(const PriorityQueue& pq);
	~PriorityQueue();

	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue();

	T peek() const;
};

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	sListPtr = new LinkedSortedList();
}


template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue& pq)
{
	sListPtr = new LinkedSortedList(*pq.sListPtr);
}

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	sListPtr->~LinkedSortedList();
}


template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return sListPtr->isEmpty();
}


template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry)
{
	sListPtr->insertSorted(newEntry);
	return true;
}


template <typename T>
bool PriorityQueue<T>::dequeue()
{
	return sListPtr->removes(sListPtr->getLength());
}


template <typename T>
T PriorityQueue<T>::peek() const
{
	return sListPtr->getNodeAt(sListPtr->getLength());
}

#endif 