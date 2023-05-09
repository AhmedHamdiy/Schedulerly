#include <iostream>
#include <algorithm>
#define MAX 1000
template <typename T>
class pnode
{
public:
    T data;
    int pri;
    pnode operator = (const pnode<T> other)
    {
        data = other.data;
        pri = other.pri;
        return *this;
    }
};
template <typename T>

class PriorityQueue {
public:
    PriorityQueue() : size(0) {}

    void enqueue(T& value,int pri) {
        if (size >= MAX) {
            throw std::overflow_error("Priority queue is full");
            return;
        }
        pnode<T>* NewNode = new pnode<T>;
            NewNode->data = value;
        NewNode->pri = pri;
        heap[size]=*NewNode;
        size++;
        HeapifyUp(size - 1);
    }

    void dequeue() {
           if(!isEmpty())
           {
               size--;
               swap(heap[0], heap[size]);
               HeapifyDown(0);
           }
    }

    T Peek() const {
       if(!isEmpty())
       {
           pnode<T> top = heap[0];
           return top.data;
       }
    }

    bool isEmpty() const {
        return size == 0;
    }

    void Print() {
        heapsort(heap, size);
        for (int i = 0; i < size; i++) {
            pnode<T> c = heap[i];
            cout << c.data << " ";
        }
        cout << endl;
    }
    int getCount()
    {
        return size;
    }
private:
    pnode<T> heap[MAX];
    int size;

    void HeapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            pnode<T> my = heap[i];
            pnode<T>par = heap[parent];
            if (my.pri <par.pri ) {
                swap(heap[i], heap[parent]);
                i = parent;
            }
            else {
                break;
            }
        }
    }

    void HeapifyDown(int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int min_child = i;
            pnode<T> mym = heap[left];
            pnode<T>par = heap[min_child];
            if (left < size && mym.pri < par.pri) {
                min_child = left;
            }
            pnode<T> my = heap[right];
            pnode<T>par2 = heap[min_child];
            if (right < size && my.pri < par2.pri) {
                min_child = right;
            }
            if (min_child != i) {
                swap(heap[i], heap[min_child]);
                i = min_child;
            }
            else {
                break;
            }
        }
    }

    void heapify(pnode<T> arr[], int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        pnode<T> lft = heap[left];
        pnode<T>lar = heap[largest];
        
        if (left < n && lft.pri >lar.pri) {
            largest = left;
        }
        pnode<T>lr = heap[largest];
        pnode<T>rgt = heap[right];

        if (right < n && rgt.pri > lr.pri) {
            largest = right;
        }

        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }

    void heapsort(pnode<T> arr[], int n) {
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }

        for (int i = n - 1; i >= 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }
    void swap(pnode<T>& first, pnode<T>& second)
    {
        pnode<T> temp = first;
        first = second;
        second = temp;
    }
    
};

