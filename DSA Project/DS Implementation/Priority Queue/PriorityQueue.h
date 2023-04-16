#pragma once
#include<iostream>
#define MAX 400
using namespace std;
template <class ItemType>
class PriorityQueue {  //This Priority Queue Is Implemented Using Min Heap
private:
	int size;
	ItemType Items[MAX];
	int free;
	//Utiliy Functions To Get The Index 
	int LChild_Indx(int i) { return (2 * i) + 1; }
	int RChild_Indx(int i) { return 2 * (i + 1); }
	int Parent_Indx(int i) { return (i - 1) / 2; }
	void Swap(ItemType& a, ItemType& b);
	//A Function To Heapify The Queue After Insertion
	void HeapifyUp(int index);
	//A Function To Heapify The Queue After Deletion
	void HeapifyDown(int index);
public:
	PriorityQueue();
	int getsize() const { return size; };
	bool enqueue(const ItemType& entry);
	bool dequeue();
	ItemType peekFront()const;
	bool isEmpty()const;
	void Print()const;
};

template<class ItemType>
inline PriorityQueue<ItemType>::PriorityQueue() { size = 0; }


template<class ItemType>
inline void PriorityQueue<ItemType>::HeapifyDown(int index) {

	int Rchild_idx = RChild_Indx(index);
	int Lchild_idx = LChild_Indx(index);
	int SmallestChild_idx = index;

	if (Lchild_idx < size && Items[Lchild_idx] < Items[SmallestChild_idx]) {
		SmallestChild_idx = Lchild_idx;
	}
	else if (Rchild_idx < size && Items[Rchild_idx] < Items[SmallestChild_idx]) {
		SmallestChild_idx = Rchild_idx;
	}
	if (SmallestChild_idx != index)
	{
		Swap(Items[SmallestChild_idx], Items[index]);
		HeapifyDown(SmallestChild_idx);
	}
}


template<class ItemType>
inline void PriorityQueue<ItemType>::HeapifyUp(int index) {
	 int parentIndex = (index - 1) / 2;

        while (index > 0 && Items[parentIndex] > Items[index]) {
            Swap(Items[parentIndex], Items[index]);
            index = parentIndex;
            parentIndex = (index - 1) / 2;
        }
}

template<class ItemType>
inline void PriorityQueue<ItemType>::Swap(ItemType& a, ItemType& b) {
	ItemType temp = a;
	a = b;
	b = temp;
}



template<class ItemType>
inline bool PriorityQueue<ItemType>::enqueue(const ItemType& entry) {
	if (size < MAX)
	{
		Items[size] = entry;
		HeapifyUp(size);
		size++;
		return true;
	}
	//There is an Overflow
	return false;
}



template<class ItemType>
inline bool PriorityQueue<ItemType>::dequeue() {
	if (!isEmpty())
	{
		Items[0] = Items[size - 1];
		size--;
		HeapifyDown(0);
		return true;
	}
	return false;
}

template<class ItemType>
inline ItemType PriorityQueue<ItemType>::peekFront()const {
	if (!isEmpty())
		return Items[0];
}

template<class ItemType>
inline bool PriorityQueue<ItemType>::isEmpty()const {
	return size == 0;
}
template<class ItemType>
inline void PriorityQueue<ItemType>::Print()const {
	for (int i = 0; i < size; i++)
		cout << Items[i]<<" ";
	cout << endl;
}