
// TODO: WRITE YOUR NAME AND ID
// STUDENT NAME: Ahmed Hamdy Mohammed Abdallah
// STUDENT ID: 9220032

#define _CRT_SECURE_NO_WARNINGS
#include <string>

#include <iostream>
#include "string"
using namespace std;

// =========================== STACK Code ==================================

template<typename T>
class ArrayStack  //Dont change this
{
private:
	T* items;		// Array of stack items
	int      top;                   // Index to top of stack
	const int STACK_SIZE;

public:

	ArrayStack(int MaxSize) : STACK_SIZE(MaxSize)
	{
		items = new T[STACK_SIZE];
		top = -1;
	}  // end default constructor

	//Function getCapacity() returns the stack max size
	//added for array implementaion only // (noon)
	int getCapacity()
	{
		return STACK_SIZE;
	}

	bool isEmpty() const
	{
		return top == -1;
	}  // end isEmpty

	bool push(const T& newEntry)
	{
		if (top == STACK_SIZE - 1) return false;	//Stack is FULL

		top++;
		items[top] = newEntry;
		return true;
	}  // end push

	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		top--;
		return true;
	}  // end pop

	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}  // end peek

	//Destructor
	~ArrayStack()
	{
		delete[]items;
	}

	//Copy constructor
	ArrayStack(const ArrayStack<T>& S) :STACK_SIZE(S.STACK_SIZE)
	{
		items = new T[STACK_SIZE];
		for (int i = 0; i <= S.top; i++)
			items[i] = S.items[i];
		top = S.top;
	}

	void PrintStack()
	{
		ArrayStack<T> temp(STACK_SIZE);
		T x;
		while (pop(x))
		{
			cout << x << " ";
			temp.push(x);
		}
		cout << endl;

		while (temp.pop(x))
		{
			push(x);
		}
	}

}; // end ArrayStack

// =========================== QUEUE Code ==================================

template < typename T>
class Node
{
private:
	T item; // A data item
	Node<T>* next; // Pointer to next node
public:

	Node()
	{
		next = nullptr;
	}

	Node(const T& r_Item)
	{
		item = r_Item;
		next = nullptr;
	}

	Node(const T& r_Item, Node<T>* nextNodePtr)
	{
		item = r_Item;
		next = nextNodePtr;
	}
	void setItem(const T& r_Item)
	{
		item = r_Item;
	}

	void setNext(Node<T>* nextNodePtr)
	{
		next = nextNodePtr;
	}

	T getItem() const
	{
		return item;
	}

	Node<T>* getNext() const
	{
		return next;
	}

};

template <typename T>
class LinkedQueue
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
public:

	LinkedQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;

	}

	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}

	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
			frontPtr = newNodePtr; // The queue is empty
		else
			backPtr->setNext(newNodePtr); // The queue was not empty

		backPtr = newNodePtr; // New node is the last node now
		return true;
	} // end enqueue

	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		return true;
	}

	bool peek(T& frntEntry) const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;
	}

	~LinkedQueue()
	{
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}

	// Copy Constructor
	LinkedQueue(const LinkedQueue<T>& LQ)
	{
		Node<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			frontPtr = backPtr = nullptr;
			return;
		}

		//insert the first node
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		frontPtr = backPtr = ptr;
		NodePtr = NodePtr->getNext();

		//insert remaining nodes
		while (NodePtr)
		{
			Node<T>* ptr = new Node<T>(NodePtr->getItem());
			backPtr->setNext(ptr);
			backPtr = ptr;
			NodePtr = NodePtr->getNext();
		}
	}

};

template < typename T>
void PrintQueue(LinkedQueue<T> Q)
{
	T K;
	while (Q.dequeue(K))
		cout << K << " ";
	cout << endl;
}
//end of queue class

// =========================== The Required Function ==================================


// TODO
void Execute(LinkedQueue<char>& input)
{
	// TODO: implement this function and don't change its prototype
	if (input.isEmpty())
		return;
	int cntr = 0;
	LinkedQueue<char> aux_q;
	ArrayStack<char> aux_S(2000);
	LinkedQueue<char>tmp_que;
	while (!input.isEmpty())
	{
		char c;
		input.dequeue(c);
		if (!isdigit(c) && c != '[' && c != '-' && c != '%')
		{
			aux_S.push(c);
			cntr++;
		}
		else if (c == '[')
		{
			char z;
			input.dequeue(z);
			int x = int(z)-48;
			if (x <= cntr)
			{
				ArrayStack<char> tmp_S(10);
				for (int i = 0; i < x; i++)
				{
					char w;
					aux_S.pop(w);
					cntr--;
					tmp_S.push(w);
				}
				ArrayStack<char> tmp1_S(tmp_S);
				for (int i = 0; i < x; i++)
				{
					char u;
					tmp_S.pop(u);
					aux_S.push(u);
					cntr++;
				}
				for (int i = 0; i < x; i++)
				{
					char u;
					tmp1_S.pop(u);
					aux_S.push(u);
					cntr++;
				}
			}
			input.dequeue(z);
		}
		else if (c == '-')
		{
			char z;
			input.dequeue(z);
			int x = int(z) - 48;
			if (x == 0)
			{
					continue;
			}
			if (x <= cntr)
			{
				ArrayStack<char> tmp_S(10);
				for (int i = 1; i < x; i++)
				{
					char w;
					aux_S.pop(w);
					cntr--;
					tmp_S.push(w);
				}
				char h;
				aux_S.pop(h);
				for (int i = 1; i < x; i++)
				{
					char u;
					tmp_S.pop(u);
					aux_S.push(u);
					cntr++;
				}
			}

		}
		else if (c == '%')
		{
			char z;
			input.dequeue(z);
			int x = int(z)-48;
			if (x == 0)
			{
				continue;
			}
			if (x <= cntr)
			{
				ArrayStack<char> tmp_S(10);
				ArrayStack<char> tmp_Stack(10);
				for (int i = 0; i < x; i++)
				{
					char w;
					aux_S.pop(w);
					cntr--;
					if(!tmp_S.isEmpty())
					{
						char o;
						if (w > tmp_S.peek(o))
						{
							char x;
							tmp_S.pop(x);
							tmp_Stack.push(w);
						}
						tmp_Stack.push(x);
					}
					else
						tmp_Stack.push(w);
				}
				for (int i = 0; i < x; i++)
				{
					char u;
					tmp_Stack.pop(u);
					tmp_S.push(u);
				}
				for (int i = 0; i < x; i++)
				{
					char u;
					tmp_S.pop(u);
					aux_S.push(u);
					cntr++;
				}
			}

		}
	}
	ArrayStack<char> t_stack(2000);
	while (!aux_S.isEmpty())
	{
		char t;
		aux_S.pop(t);
		t_stack.push(t);
	}
	while (!t_stack.isEmpty())
	{
		char element;
		t_stack.pop(element);
		input.enqueue(element);
	}


}


// =========================== Main ==================================

int main()
{
	// Note: Don't change the main function.
	int n;
	char element;
	cin >> n;
	LinkedQueue<char> input;
	for (int i = 0; i < n; i++)
	{
		cin >> element;
		input.enqueue(element);
	}
	Execute(input);
	PrintQueue(input);

	return 0;
}






















// noon