#ifndef LinkedSortedList_h
#define LinkedSortedList_h

#include "Node.h"

template <typename T>
class LinkedSortedList
{
private:
	Node<T>* headPtr;
	int itemCount;
	Node<T>* getNodeBefore(const T& anEntry) const;
	Node<T>* copyChain(const Node<T>* originalChainPtr);
public:
	LinkedSortedList();
	LinkedSortedList(const LinkedSortedList<T>& aList);
	~LinkedSortedList();

	void insertSorted(const T& newEntry);
	bool removeSorted(const T& anEntry);
	int getPosition(const T& newEntry) const;
	Node<T>* getNodeAt(int position) const;

	bool isEmpty() const;
	int getLength() const;
	bool remove(int position);
	void clear();
	T getEntry(int position);
};

template <typename T>
Node<T>* LinkedSortedList<T>::getNodeBefore(const T& anEntry) const
{
	Node<T>* curPtr = headPtr;
	Node<T>* prevPtr = nullptr;
	while ((curPtr != nullptr) && (anEntry > curPtr->getItem()))
	{
		prevPtr = curPtr;
		curPtr = curPtr->getNext();
	}

	return prevPtr;
}

template <typename T>
Node<T>* LinkedSortedList<T>::getNodeAt(int position) const
{
	Node<T>* curPtr = headPtr;
	for (int i(0); i < position; i++)
		curPtr = curPtr->getNext();

	return curPtr;
}


template <typename T>
Node<T>* LinkedSortedList<T>::copyChain(const Node<T>* originalChainPtr)
{
	Node<T>* copiedChainPtr;
	if (originlChainPtr == nullptr)
	{
		copiedChainPtr = nullptr;
		itemCount = 0;
	}
	else
	{
		Node<T>* copiedChainPtr = new Node<T>(originalChainPtr->getItem());
		copiedChainPtr->setNext(copyChain(originalChainPtr->getNext()));
		itemCount++;
	}
	return copiedChainPtr;
}


template <typename T>
LinkedSortedList<T>::LinkedSortedList()
	:headPtr(nullptr), itemCount(0)
{}


template <typename T>
LinkedSortedList<T>::LinkedSortedList(const LinkedSortedList<T>& aList)
{
	headPtr = copyChain(aList.headPtr);
}

template <typename T>
LinkedSortedList<T>::~LinkedSortedList()
{
	clear();
}

template <typename T>
void LinkedSortedList<T>::insertSorted(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	Node<T>* prevPtr = getNodeBefore(newEntry);

	if (isEmpty() || (prevPtr == nullptr))
	{
		newNodePtr->setNext(headPtr);
		headPtr = newNodePtr;
	}
	else
	{
		Node<T>* aftPtr = prevPtr->getNext();
		newNodePtr->setNext(aftPtr);
		prevPtr->setNext(newNodePtr);
	}

	itemCount++;
}

template <typename T>
bool LinkedSortedList<T>::removeSorted(const T& anEntry)
{
	if (getPosition() == -1)
		return false;
	remove(anEntry);
	return true;
}

template <typename T>
int LinkedSortedList<T>::getPosition(const T& newEntry) const
{
	if (isEmpty())
		return -1;
	int i(1);
	Node<T>* curPtr = headPtr;
	while (curPtr != nullptr)
	{
		if (curPtr->getItem() > newEntry)
			return -1;
		if (curPtr->getItem() == newEntry)
			return i;
		i++;
		curPtr = curPtr->getNext();
	}
}

template <typename T>
bool LinkedSortedList<T>::isEmpty() const
{
	return ((headPtr == nullptr) && (itemCount == 0));
}

template <typename T>
int LinkedSortedList<T>::getLength() const
{
	return itemCount;
}

template <typename T>
bool LinkedSortedList<T>::remove(int position)
{
	bool ableToRemove = (position >= 1) && (position <= itemCount);
	if (ableToRemove)
	{
		Node<T>* curPtr = nullptr;
		if (position == 1)
		{
			curPtr = headPtr;
			headPtr = headPtr->getNext();
		}
		else
		{
			Node<T>* prevPtr = getNodeAt(position - 1);
			curPtr = prevPtr->getNext();
			prevPtr->setNext(curPtr->getNext());
		}

		curPtr->setNext(nullptr);
		delete curPtr;
		curPtr = nullptr;

		itemCount--;
	}

	return ableToRemove;
}

template <typename T>
void LinkedSortedList<T>::clear()
{
	while (!isEmpty())
		remove(1);
}

template <typename T>
T LinkedSortedList<T>::getEntry(int position)
{
	Node<T>* nodePtr = getNodeAt(position);
	return nodePtr->getItem();
}


#endif 