#pragma once
#include<iostream>
#include "Node.h"
using namespace std;
template<class ItemType>
class LinkedQueue
{
private:
	// The queue is implemented as a chain of linked nodes that has
	// two external pointers, a head pointer for the front of the
	// queue and a tail pointer for the back of the queue.
	Node<ItemType>* backPtr;
	Node<ItemType>* frontPtr;
	int count;
public:
	LinkedQueue();
	LinkedQueue(const LinkedQueue& aQueue);
	virtual ~LinkedQueue();
	void print()const;
	bool isEmpty() const;
	bool enqueue(const ItemType& newEntry);
	bool dequeue(ItemType& frntEntry);
	ItemType peekFront() const;
	int getcount() const;
	void clear();
};

template<class ItemType>
inline LinkedQueue<ItemType>::LinkedQueue()
	: backPtr(nullptr), frontPtr(nullptr)
{
	count = 0;
}

template<class ItemType>
inline LinkedQueue<ItemType>::LinkedQueue(const LinkedQueue& aQueue)
{
	Node<ItemType>* originChainPtr = aQueue.frontPtr;

	if (originChainPtr == nullptr)
	{
		frontPtr = nullptr;
		backPtr = nullptr;
		count = 0;
		return;
	}

	while (originChainPtr != nullptr)
	{
		enqueue(originChainPtr->getItem());
		originChainPtr = originChainPtr->getNext();
	}
	count = aQueue.count;
}

template <class ItemType>
LinkedQueue<ItemType>::~LinkedQueue()
{
	clear();
}

template<class ItemType>
inline bool LinkedQueue<ItemType>::isEmpty() const
{
	return frontPtr == nullptr;
}

template<class ItemType>
inline bool LinkedQueue<ItemType>::enqueue(const ItemType& newEntry)
{
	Node<ItemType>* newNodePtr = new Node<ItemType>(newEntry);

	if (isEmpty())
		frontPtr = newNodePtr;
	else
		backPtr->setNext(newNodePtr);
	backPtr = newNodePtr;
	count++;
	return true;
}

template <typename ItemType>
inline bool LinkedQueue<ItemType>::dequeue(ItemType& frntEntry)
{
	if (isEmpty())
		return false;

	Node<ItemType>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;
	count--;
	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}

template<class ItemType>
inline ItemType LinkedQueue<ItemType>::peekFront() const
{
	return frontPtr->getItem();	// didn't check for empty because it's in the specification as a @pre
}

template<class ItemType>
inline int LinkedQueue<ItemType>::getcount() const
{
	return count;
}

template<class ItemType>
inline void LinkedQueue<ItemType>::clear()
{
	ItemType temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));

}


template<class ItemType>
inline void LinkedQueue<ItemType>::print()const
{
	Node<ItemType>* Myptr = frontPtr;
	while (Myptr)
	{
		cout << Myptr->getItem() << " ";
		Myptr = Myptr->getNext();
	}
	cout << endl;
}
