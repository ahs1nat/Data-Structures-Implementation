#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

class Heap
{
private:
    int *arr;
    int maxSize;
    int currentSize;
    int parent(int i)
    {
        return (i - 1) / 2;
    }
    int leftChild(int i)
    {
        return (2 * i) + 1;
    }
    int rightChild(int i)
    {
        return (2 * i) + 2;
    }
    void heapify(int i)
    {
        while (true)
        {
            int left = leftChild(i);
            int right = rightChild(i);
            int largest;
            if (left < currentSize && arr[left] > arr[i])
            {
                largest = left;
            }
            else
                largest = i;
            if (right < currentSize && arr[right] > arr[largest])
            {
                largest = right;
            }
            if (largest != i)
            {
                std::swap(arr[i], arr[largest]);
                i = largest;
            }
            else
                break;
        }
    }

public:
    Heap(int maxSize)
    {
        this->maxSize = maxSize;
        arr = new int[maxSize];
        currentSize = 0;
    }
    Heap(const std::vector<int> &v)
    {
        currentSize = 0;
        maxSize = v.size();
        arr = new int[maxSize];
        for (int i = 0; i < v.size(); i++)
        {
            insert(v[i]);
        }
    }
    ~Heap() { delete[] arr; }
    void insert(int value)
    {
        if (currentSize == maxSize)
            throw std::overflow_error("Heap is full");
        int i = currentSize;
        arr[i] = value;
        while (i > 0 && arr[parent(i)] < arr[i])
        {
            swap(arr[parent(i)], arr[i]);
            i = parent(i);
        }
        currentSize++;
    }
    int size()
    {
        return currentSize;
    }
    int getMax()
    {
        if (currentSize == 0)
            throw std::underflow_error("Heap is empty");
        return arr[0];
    }
    void deleteKey()
    {
        if (currentSize == 0)
            throw std::underflow_error("Heap is empty");
        arr[0] = arr[currentSize - 1];
        currentSize--;
        heapify(0);
    }
};
void heapsort(std::vector<int> &v)
{
    Heap h(v);
    v.clear();
    while (h.size() > 0)
    {
        v.push_back(h.getMax());
        h.deleteKey();
    }
}

#endif