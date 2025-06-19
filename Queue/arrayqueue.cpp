#include "queue.h"
#include <iostream>
using namespace std;
// Constructor implementation
ArrayQueue::ArrayQueue(int initial_capacity)
{
    // TODO: Initialize data members (data, capacity, front_idx, rear_idx)
    // TODO: Allocate memory for the array with the specified initial capacity
    capacity = initial_capacity;
    data = new int[capacity];
    front_idx = 0;
    rear_idx = 0;
}

// Destructor implementation
ArrayQueue::~ArrayQueue()
{
    // TODO: Free the dynamically allocated memory for the array
    delete[] data;
}

// Enqueue implementation (add an item to the rear of the queue)
void ArrayQueue::enqueue(int item)
{
    // TODO: Check if the array is full
    // TODO: If full, resize the array to double its current capacity
    // TODO: Add the new element to the rear of the queue
    if (size() == capacity)
    {
        resize(2 * capacity);
    }
    data[rear_idx % capacity] = item;
    rear_idx++;
}

// Dequeue implementation (remove an item from the front of the queue)
int ArrayQueue::dequeue()
{
    // TODO: Check if the queue is empty, display error message if it is
    // TODO: Decrement current_size and return the element at the front of the queue
    // TODO: Update front index
    // TODO: Return the dequeued element
    if (empty())
    {
        cout << "Queue is empty." << endl;
        return -1;
    }
    int value = data[front_idx % capacity];
    front_idx++;
    if (empty())
    {
        front_idx = 0;
        rear_idx = 0;
    }
    if (size() <= capacity / 4 && capacity > 2)
    {
        resize(max(2,capacity / 2));
    }
    return value;
}

// Clear implementation
void ArrayQueue::clear()
{
    // TODO: Reset the queue to be empty (reset capacity, front_idx, rear_idx, data)
    delete[] data;
    capacity = 2;
    data = new int[capacity];
    front_idx = 0;
    rear_idx = 0;
}

// Size implementation
int ArrayQueue::size() const
{
    // TODO: Return the number of elements currently in the queue
    return rear_idx - front_idx;
}

// Front implementation
int ArrayQueue::front() const
{
    // TODO: Check if the queue is empty, display error message if it is
    // TODO: Return the element at the front of the queue without removing it
    if (empty())
    {
        cout << "Queue is empty." << endl;
        return -1;
    }
    return data[front_idx % capacity];
}

// Back implementation (get the element at the back of the queue)
int ArrayQueue::back() const
{
    // TODO: Check if the queue is empty, display error message if it is
    // TODO: Return the element at the back of the queue without removing it
    if (empty())
    {
        cout << "Queue is empty." << endl;
        return -1;
    }
    return data[(rear_idx - 1) % capacity];
}

// Empty implementation
bool ArrayQueue::empty() const
{
    // TODO: Return whether the queue is empty (current_size == 0)
    return size() == 0;
}

// Print implementation
string ArrayQueue::toString() const
{
    // TODO: Convert queue to a string representation in the format: <elem1, elem2, ..., elemN|
    if (empty())
    {
        return "<|";
    }
    string result = "<";
    int n = size();
    for (int i = 0; i < n; i++)
    {
        result += to_string(data[(front_idx + i) % capacity]);
        if (i != n - 1)
            result += ", ";
    }
    result += "|";
    return result;
}

// Resize implementation
void ArrayQueue::resize(int new_capacity)
{
    // TODO: Create a new array with the new capacity
    // TODO: Copy elements from the old array to the new array
    // TODO: Delete the old array
    // TODO: Update the data pointer and capacity
    // TODO: Update front and rear indices
    int n = size();
    int *temp = new int[new_capacity];
    for (int i = 0; i < n; i++)
    {
        temp[i] = data[(front_idx + i) % capacity];
    }
    delete[] data;
    data = temp;
    capacity = new_capacity;
    front_idx = 0;
    rear_idx = n;
}

int ArrayQueue::getCapacity() const
{
    // TODO: Return the current capacity of the queue
    return capacity;
}