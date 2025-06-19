#include <iostream>
#include <vector>
#include "queue.h"
using namespace std;

/********************************* RANDOM ************************************/
#define LCG_MULTIPLIER (1103515245)
#define LCG_INCREMENT (12345)
#define LCG_MODULUS ((unsigned)~0 >> 1)
static unsigned int lcg_seed = 1;
void custom_srand(unsigned int seed)
{
    lcg_seed = seed;
}
int custom_rand()
{
    lcg_seed = (LCG_MULTIPLIER * lcg_seed + LCG_INCREMENT) % (LCG_MODULUS + 1);
    return lcg_seed;
}
int randomQueue(int seed = -1)
{
    static bool initialized = false;
    if (seed != -1 && !initialized)
    {
        custom_srand(seed);
        initialized = true;
        return -1;
    }
    cout << "randomQueue() called" << endl;
    return (custom_rand() % 2) + 1;
}
/*****************************************************************************/

int timestamps[10001] = {0};
bool merged = false;

int main()
{
    //freopen("test_input_1.txt", "r", stdin); // Test Case 1
    freopen("test_input_2.txt", "r", stdin); // Test Case 2
    freopen("output.txt", "w", stdout);
    // Initialize the random generator with a fixed seed
    // You should set the seed only once at the beginning of your program
    // NOTE: Do not modify the following lines.
    randomQueue(42);

    Queue *Q1 = new ListQueue();
    Queue *Q2 = new ListQueue();
    Queue *Q = new ArrayQueue();

    int N;
    cin >> N;

    for (int i = 1; i <= N; i++)
    {
        // TODO: Edit here to add your own logic
        int op;
        cin >> op;
        if (op == 1)
        {
            int id, timestamp;
            cin >> id >> timestamp;
            cout << "Operation " << i << " (Arrival " << id << " " << timestamp << "): ";
            timestamps[id] = timestamp;
            if (merged)
            {
                Q->enqueue(id);
                cout << endl;
            }
            else
            {
                if (Q1->empty() && Q2->empty())
                {
                    int choice = randomQueue();
                    if (choice == 1)
                        Q1->enqueue(id);
                    else
                        Q2->enqueue(id);
                }
                else if (Q1->empty())
                {
                    Q1->enqueue(id);
                    cout << endl;
                }
                else if (Q2->empty())
                {
                    Q2->enqueue(id);
                    cout << endl;
                }
                else
                {
                    int q1_back = Q1->back();
                    int q2_back = Q2->back();
                    if (timestamps[q1_back] <= timestamps[q2_back])
                    {
                        Q1->enqueue(id);
                    }
                    else
                    {
                        Q2->enqueue(id);
                    }
                    cout << endl;
                }
            }
        }
        else if (op == 2)
        {
            int id, timestamp;
            cin >> id >> timestamp;
            cout << "Operation " << i << " (Leave " << id << " " << timestamp << "): " << endl;
            int size, v;
            bool found = false;
            if (merged)
            {
                if (!Q->empty())
                {
                    size = Q->size();
                    for (int j = 0; j < size; j++)
                    {
                        v = Q->dequeue();
                        if (v != id)
                            Q->enqueue(v);
                        else
                            found = true;
                    }
                }
            }
            else
            {
                if (!Q1->empty())
                {
                    size = Q1->size();
                    for (int j = 0; j < size; j++)
                    {
                        v = Q1->dequeue();
                        if (v != id)
                            Q1->enqueue(v);
                        else
                            found = true;
                    }
                }
                else if (!Q2->empty() && !found)
                {
                    size = Q2->size();
                    for (int j = 0; j < size; j++)
                    {
                        v = Q2->dequeue();
                        if (v != id)
                            Q2->enqueue(v);
                        else
                            found = true;
                    }
                }
            }
            if (!found)
                cout << "Patient " << id << "not found" << endl;
        }
        else if (op == 3)
        {
            if (!merged)
            {
                cout << "Operation " << i << " (Merge): " << endl;
                merged = true;
                Queue *temp = new ArrayQueue();
                while (!Q1->empty() || !Q2->empty())
                {
                    if (Q1->empty())
                    {
                        temp->enqueue(Q2->dequeue());
                    }
                    else if (Q2->empty())
                    {
                        temp->enqueue(Q1->dequeue());
                    }
                    else
                    {
                        int q1_front = Q1->front();
                        int q2_front = Q2->front();
                        if (timestamps[q1_front] <= timestamps[q2_front])
                        {
                            temp->enqueue(Q1->dequeue());
                        }
                        else
                        {
                            temp->enqueue(Q2->dequeue());
                        }
                    }
                }
                while (!temp->empty())
                {
                    Q->enqueue(temp->dequeue());
                }
                delete temp;
            }
            else
            {
                cout << "Q1 and Q2 are already merged." << endl;
            }
        }
        else if (op == 4)
        {
            if (merged)
            {
                cout << "Operation " << i << " (Split): " << endl;
                merged = false;
                int position = 1;
                while (!Q->empty())
                {
                    int v = Q->dequeue();
                    if (position % 2 == 1)
                    {
                        Q1->enqueue(v);
                    }
                    else
                    {
                        Q2->enqueue(v);
                    }
                    position++;
                }
            }
            else
            {
                cout << "Q1 and Q2 are not merged" << endl;
            }
        }
        else if (op == 5)
        {
            cout << "Operation " << i << " (Departure): ";
            if (merged)
            {
                if (!Q->empty())
                {
                    int id = Q->dequeue();
                    timestamps[id] = 0;
                }
                cout << endl;
            }
            else
            {
                if (Q1->empty() && !Q2->empty())
                {
                    int id = Q2->dequeue();
                    timestamps[id] = 0;
                    cout << endl;
                }
                else if (!Q1->empty() && Q2->empty())
                {
                    int id = Q1->dequeue();
                    timestamps[id] = 0;
                    cout << endl;
                }
                else if (!Q1->empty() && !Q2->empty())
                {
                    int queue_choice = randomQueue();
                    if (queue_choice == 1)
                    {
                        int id = Q1->dequeue();
                        timestamps[id] = 0;
                    }
                    else
                    {
                        int id = Q2->dequeue();
                        timestamps[id] = 0;
                    }
                }
            }
        }

        cout << "Q1: " << Q1->toString() << endl;
        cout << "Q2: " << Q2->toString() << endl;
        cout << "Q : " << Q->toString() << endl;

        // After each operation, we will check the queue's size and capacity
        // NOTE: Do not modify the following lines.

        int capacity = ((ArrayQueue *)Q)->getCapacity();
        if ((Q->size() < capacity / 4 && capacity > 2))
        {
            cout << "Error: Queue size is less than 25% of its capacity." << endl;
        }
        else if (capacity < 2)
        {
            cout << "Error: Queue capacity is less than 2." << endl;
        }
    }
    delete Q1;
    delete Q2;
    delete Q;

    return 0;
}