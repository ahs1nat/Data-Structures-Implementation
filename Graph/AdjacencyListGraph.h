#ifndef ADJACENCY_LIST_GRAPH_H
#define ADJACENCY_LIST_GRAPH_H

#include <iostream>
#include "GraphADT.h"
#include "linkedList.h"
#include "queue.h"

class AdjacencyListGraph : public GraphADT
{
private:
    // TODO: Take necessary private members
    linkedList *adjLGraph;
    int nodeCount;
    int capacity;

    int findNodeIndex(int v) const
    {
        for (int i = 0; i < nodeCount; i++)
        {
            if (adjLGraph[i].head != nullptr && adjLGraph[i].head->element == v)
            {
                return i;
            }
        }
        return -1;
    }

public:
    AdjacencyListGraph()
    {
        capacity = 100;
        nodeCount = 0;
        adjLGraph = new linkedList[capacity];
        for (int i = 0; i < capacity; i++)
        {
            init(&adjLGraph[i]);
        }
    }
    ~AdjacencyListGraph()
    {
        for (int i = 0; i < nodeCount; i++)
        {
            free_list(&adjLGraph[i]);
        }
        delete[] adjLGraph;
    }
    void AddNode(int v) override
    {
        // TODO: Add a new node v and resize the matrix if your current matrix is almost going to be full.
        if (findNodeIndex(v) == -1)
        {
            if (nodeCount >= capacity)
            {
                capacity *= 2;
                linkedList *temp = new linkedList[capacity];
                for (int i = 0; i < capacity; i++)
                {
                    init(&temp[i]);
                    if (i < nodeCount)
                    {
                        temp[i] = adjLGraph[i];
                    }
                }
                delete[] adjLGraph;
                adjLGraph = temp;
            }
            init(&adjLGraph[nodeCount]);
            append(v, &adjLGraph[nodeCount]);
            nodeCount++;
        }
    }

    void AddEdge(int u, int v) override
    {
        // Automatically add the nodes if they don't exist
        int u_index = findNodeIndex(u);
        if (u_index == -1)
        {
            AddNode(u);
            u_index = findNodeIndex(u); // update after addition
        }
        int v_index = findNodeIndex(v);
        if (v_index == -1)
        {
            AddNode(v);
            v_index = findNodeIndex(v);
        }
        // Add the edge if not already present
        if (!is_present(v, &adjLGraph[u_index]))
        {
            append(v, &adjLGraph[u_index]);
        }
        if (!is_present(u, &adjLGraph[v_index]))
        {
            append(u, &adjLGraph[v_index]);
        }
    }

    bool CheckEdge(int u, int v) const override
    {
        // TODO: Check whether there is an edge between two nodes u and v
        int u_index = findNodeIndex(u);
        int v_index = findNodeIndex(v);
        if (u_index != -1 && v_index != -1)
        {
            return is_present(u, &adjLGraph[v_index]);
        }
        return false;
    }

    void RemoveNode(int v) override
    {
        // TODO: Remove a node.
        int v_index = findNodeIndex(v);
        if (v_index != -1)
        {
            for (int i = 0; i < nodeCount; i++)
            {
                if (i != v_index)
                {
                    delete_item(v, &adjLGraph[i]);
                }
            }
            free_list(&adjLGraph[v_index]);
            for (int i = v_index; i < nodeCount - 1; i++)
            {
                adjLGraph[i] = adjLGraph[i + 1];
            }
            init(&adjLGraph[nodeCount - 1]);
            nodeCount--;
        }
    }

    void RemoveEdge(int u, int v) override
    {
        // TODO: remove an edge
        int u_index = findNodeIndex(u);
        int v_index = findNodeIndex(v);
        if (u_index != -1 && v_index != -1)
        {
            delete_item(v, &adjLGraph[u_index]);
            delete_item(u, &adjLGraph[v_index]);
        }
    }

    bool CheckPath(int u, int v) const override
    {
        // TODO: Return true if there is a path between nodes u and v. Otherwise return false
        int u_index = findNodeIndex(u);
        int v_index = findNodeIndex(v);
        if (u_index == -1 || v_index == -1)
        {
            return false;
        }
        bool *visited = new bool[nodeCount];
        for (int i = 0; i < nodeCount; i++)
        {
            visited[i] = false;
        }
        ListQueue q;
        q.enqueue(u);
        visited[u_index] = true;

        while (!q.empty())
        {
            int curr = q.dequeue();
            if (curr == v)
            {
                delete[] visited;
                return true;
            }
            int curr_index = findNodeIndex(curr);
            node *temp = adjLGraph[curr_index].head->next;
            while (temp != nullptr)
            {
                int neighbor = temp->element;
                if (!visited[findNodeIndex(neighbor)])
                {
                    visited[findNodeIndex(neighbor)] = true;
                    q.enqueue(neighbor);
                }
                temp = temp->next;
            }
        }
        delete[] visited;
        return false;
    }

    void FindShortestPath(int u, int v) const override
    {
        // TODO: Find the shortest path between the nodes u and v and print it.
        int u_index = findNodeIndex(u);
        int v_index = findNodeIndex(v);
        if (u_index == -1 || v_index == -1)
        {
            std::cout << "No path" << std::endl;
            return;
        }
        bool *visited = new bool[nodeCount];

        int *prev = new int[nodeCount];
        for (int i = 0; i < nodeCount; i++)
        {
            visited[i] = false;
            prev[i] = -1;
        }
        ListQueue q;
        q.enqueue(u);
        visited[u_index] = true;
        bool found = false;
        while (!q.empty())
        {
            int curr = q.dequeue();
            int curr_index = findNodeIndex(curr);
            node *temp = adjLGraph[curr_index].head->next;
            while (temp != nullptr)
            {
                int neighbor = temp->element;
                int neighbor_index = findNodeIndex(neighbor);
                if (!visited[neighbor_index])
                {
                    visited[neighbor_index] = true;
                    prev[neighbor_index] = curr;
                    q.enqueue(neighbor);
                    if (neighbor == v)
                    {
                        found = true;
                        break;
                    }
                }
                temp = temp->next;
            }
            if (found)
                break;
        }
        if (!found)
        {
            std::cout << "No path" << std::endl;
            delete[] visited;
            delete[] prev;
            return;
        }
        int path[100];
        int path_len = 0;
        int current = v;

        while (current != -1)
        {
            path[path_len++] = current;
            current = prev[findNodeIndex(current)];
        }

        // Print in reverse
        std::cout << "Shortest path: ";
        for (int i = path_len - 1; i >= 0; i--)
        {
            std::cout << path[i];
            if (i > 0)
                std::cout << " ";
        }
        std::cout << "\n";

        delete[] visited;
        delete[] prev;
    }

    int FindShortestPathLength(int u, int v) const override
    {
        // TODO: Return the shortest path length between nodes u and v if any such path exists. Otherwise return -1.
        int u_index = findNodeIndex(u);
        int v_index = findNodeIndex(v);
        if (u_index == -1 || v_index == -1)
            return -1;

        bool *visited = new bool[nodeCount];
        int *distance = new int[nodeCount];
        for (int i = 0; i < nodeCount; i++)
        {
            visited[i] = false;
            distance[i] = -1;
        }

        ListQueue q;
        q.enqueue(u);
        visited[u_index] = true;
        distance[u_index] = 0;

        while (!q.empty())
        {
            int curr = q.dequeue();
            int curr_index = findNodeIndex(curr);
            node *temp = adjLGraph[curr_index].head->next;

            while (temp != nullptr)
            {
                int neighbor = temp->element;
                int neighbor_index = findNodeIndex(neighbor);
                if (!visited[neighbor_index])
                {
                    visited[neighbor_index] = true;
                    distance[neighbor_index] = distance[curr_index] + 1;
                    q.enqueue(neighbor);
                    if (neighbor == v)
                    {
                        int result = distance[neighbor_index];
                        delete[] visited;
                        delete[] distance;
                        return result;
                    }
                }
                temp = temp->next;
            }
        }
        delete[] visited;
        delete[] distance;
        return -1;
    }

    linkedList GetNeighbors(int u) const override
    {
        // TODO: Return the list of neighbors.
        linkedList neighbors;
        init(&neighbors);
        int u_index = findNodeIndex(u);
        if (u_index != -1)
        {
            node *temp = adjLGraph[u_index].head->next;
            while (temp != NULL)
            {
                append(temp->element, &neighbors);
                temp = temp->next;
            }
        }
        return neighbors;
    }
};

#endif // ADJACENCY_LIST_GRAPH_H
