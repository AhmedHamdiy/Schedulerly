#include <iostream>
#include <algorithm>
#include"../Queue/LinkedQueue.h"
#include"../../System Classes/Process.h"
class PriQueue : public LinkedQueue<Process*>
{
public:
	void enqueue(Process*& MYprocess)
	{
		Node<Process*>* newNodePtr = new Node<Process*>(MYprocess);
		if (isEmpty())
			frontPtr = newNodePtr;
		else
			backPtr->setNext(newNodePtr);
		backPtr = newNodePtr;
		count++;
	}
};