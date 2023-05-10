
// TODO: WRITE YOUR NAME AND ID
// STUDENT NAME: Ahmed Hamdy Mohammed Abdallah
// STUDENT ID: 9220032

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

//First let's declare a single node in the list
template<typename T>
class Node
{
private:
	T item;	// A data item (can be any complex sturcture)
	Node<T>* next;	// Pointer to next node  // (noon)
public:

	Node() //default constructor
	{
		next = nullptr;
	}

	Node(T newItem) //non-default constructor
	{
		item = newItem;
		next = nullptr;

	}

	void setItem(T newItem)
	{
		item = newItem;
	} // end setItem

	void setNext(Node<T>* nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext

	T getItem() const
	{
		return item;
	} // end getItem

	Node<T>* getNext() const
	{
		return next;
	}
}; // end Node


template <typename T>
class LinkedList
{
private:
	Node<T>* Head;
public:

	LinkedList()
	{
		Head = nullptr;
	}

	~LinkedList()
	{
		DeleteAll();
	}

	void PrintList()	const
	{
		Node<T>* p = Head;

		while (p)
		{
			cout << p->getItem() << " ";
			p = p->getNext();
		}
		cout << endl;
	}

	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}

	void InsertEnd(const T& data)
	{
		if (Head == nullptr)
		{
			Head = new Node<T>(data);
		}
		else
		{
			Node<T>* ptr = Head;
			while (ptr->getNext())
				ptr = ptr->getNext();
			ptr->setNext(new Node<T>(data));
		}
	}

	// =========================== The Required Function ==================================


	// TODO
	void KeepAlternating()
	{
		// TODO: implement this function and don't change its prototype
		if (!Head)
			return;
		bool flag;
		Node<T>*ptr = Head->getNext();
		if (Head->getItem() == ptr->getItem())
			Head = ptr;
		Node<T>* prev = ptr;
		while (prev->getNext())
		{
			if (prev->getItem() == prev->getNext()->getItem())
			{
				prev = prev->getNext();
			}
			if (prev->getItem() < prev->getNext()->getItem())
			{
				flag = true;
				break;
			}
			else
			{
				flag = false;
				break;
			}
		}
		while (ptr&&ptr->getNext())
		{
			if(flag)
			{
				while (ptr->getNext()&&ptr->getItem() <= ptr->getNext()->getItem())
				{
					ptr->setNext(ptr->getNext()->getNext());
				}
			}
			else
			{
				while (ptr->getNext()&&ptr->getItem() >= ptr->getNext()->getItem())
				{
					ptr->setNext(ptr->getNext()->getNext());
				}
			}
			flag = flag % 2;
			ptr = ptr->getNext();
		}
	}

};


// =========================== The Main Function ==================================

int main()
{
	// Note: Don't change the main function.
	LinkedList<int> L;
	int n, x;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> x;
		L.InsertEnd(x);
	}
	L.KeepAlternating();
	L.PrintList();
	return 0;
}






















// noon