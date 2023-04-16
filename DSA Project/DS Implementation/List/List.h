#include"../Queue/Node.h"
#include<iostream>
using namespace std;
template<typename ItemType>
class List
{
private:
	Node<ItemType>* Head;
	Node<ItemType>* Tail;
	int itemCount;
	// return A pointer to the node using the position or the data
	Node<ItemType>* getNodeAt(int position) const;
	Node<ItemType>* getPointerTo(const ItemType& target) const;
public:
	List();
	~List();						// destructor should be virtual
	int getcount() const;
	bool isEmpty() const;
	bool getEntry(int position,ItemType & temp)const;
	bool setEntry(int position, const ItemType& newEntry);
	bool insert(int position, const ItemType& newEntry);
	void insertEnd(const ItemType& data);
	bool remove(int position);
	void clear();
	void print()const;
	bool find(const ItemType& anEntry) const;
};

template<class ItemType>
bool List<ItemType>::setEntry(int position, const ItemType& newEntry)
{
	Node<ItemType>* tmp = getNodeAt(position);
	tmp->setItem(newEntry);
}

template<class ItemType>
inline bool List<ItemType>::find(const ItemType& anEntry) const {
	Node<ItemType>* tmp = getPointerTo(anEntry);
	return (tmp != nullptr);
}
template<class ItemType>
inline void List<ItemType>::print() const {
	Node<ItemType>* p = Head;
	while (p)
	{
		cout << p->getItem() << " ";
		p = p->getNext();
	}
	cout << "\n";
}

template <class ItemType>
inline bool List<ItemType>::remove(int position) {
	if ((position >= 1) && (position <= itemCount)) {
		Node<ItemType>* curPtr = nullptr;
		if (position == 1) {
			curPtr = Head;
			Head = Head->getNext();
		}
		else {
			Node<ItemType>* prevPtr = getNodeAt(position - 1);
			curPtr = prevPtr->getNext();
			prevPtr->setNext(curPtr->getNext());
			if (position == itemCount)
				Tail = prevPtr;
		}
		curPtr->setNext(nullptr);
		delete curPtr; curPtr = nullptr;
		itemCount--;
		return true;
	}
	return false;
}
template <class ItemType>
bool List<ItemType>::insert(int newPosition, const ItemType& newEntry)
{
	if ((newPosition >= 1) && (newPosition <= itemCount + 1)) {
		Node<ItemType>* newNodePtr = new Node<ItemType>(newEntry);
		if (newPosition == 1) {
			newNodePtr->setNext(Head);
			Head = newNodePtr;
		}
		else if (newPosition < itemCount) {
			Node<ItemType>* prevPtr = getNodeAt(newPosition - 1);
			newNodePtr->setNext(prevPtr->getNext());
			prevPtr->setNext(newNodePtr);
		}
		else {
			InsertEnd(newEntry);
		}
		itemCount++;
		return true;
	}
	return false;
}
template<class ItemType>
inline Node<ItemType>* List<ItemType>::getPointerTo(const ItemType& target) const
{
	Node<ItemType>* curPtr = Head;

	while (curPtr != nullptr)
	{
		if (curPtr->getItem() == target)
		{
			return curPtr;
		}
		curPtr = curPtr->getNext();
	}
	return nullptr;
}

template<class ItemType>
inline void List<ItemType>::insertEnd(const ItemType& data)
{
	Node<ItemType>* newNodePtr = new Node<ItemType>(data);
	if (!Head)
		Head = newNodePtr;
	else
		Tail->setNext(newNodePtr);
	Tail = newNodePtr;
	itemCount++;
}


template <class ItemType>
Node<ItemType>* List<ItemType>::getNodeAt(int position) const {

	if ((position >= 1) && (position <= itemCount))
	{
		Node<ItemType>* curPtr = Head;
		for (int i = 1; i < position; i++)
			curPtr = curPtr->getNext();
		return curPtr;
	}
}

template < class ItemType>
bool List<ItemType>::getEntry(int position, ItemType& temp) const //returns true if found
{
	if ((position >= 1) && (position <= itemCount))
	{
		Node<ItemType>* nodePtr = getNodeAt(position);
		temp= nodePtr->getItem(); //sending item by reference
		return 1;
	}
	return 0;
}
template <class ItemType>
void List<ItemType>::clear()
{
	while (!isEmpty())
		remove(1);
}



template<class ItemType>
inline List<ItemType>::~List()
{
	clear();
}

template<class ItemType>
inline List<ItemType>::List() :Head(nullptr), itemCount(0)
{}

template<class ItemType>
inline bool List<ItemType>::isEmpty()const
{
	return itemCount == 0;
}


template<class ItemType>
inline int List<ItemType>::getcount()const
{
	return itemCount;
}
