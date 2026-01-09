#ifndef ADJACENCY_MATRIX_GRAPH_H
#define ADJACENCY_MATRIX_GRAPH_H

#include <iostream>
#include "GraphADT.h"
#include "queue.h"
#include "linkedList.h"

class AdjacencyMatrixGraph : public GraphADT
{
private:
    // TODO: Consider necessary private members as per your discretion
    int **matrixGraph;
    int *nodeValues;
    int nodeCount;
    int capacity;

    int findNodeIndex(int v) const
    {
        for (int i = 0; i < nodeCount; i++)
        {
            if (nodeValues[i] == v)
            {
                return i;
            }
        }
        return -1;
    }

    void resizeMatrix()
    {
        // TODO: you need to resize your matrix when you will less data or more data
        if (nodeCount < capacity)
            return;

        int new_cap = capacity * 2;
        int **newMatrix = new int *[new_cap];
        for (int i = 0; i < new_cap; i++)
        {
            newMatrix[i] = new int[new_cap];
            for (int j = 0; j < new_cap; j++)
            {
                if (i < capacity && j < capacity)
                {
                    newMatrix[i][j] = matrixGraph[i][j];
                }
                else
                {
                    newMatrix[i][j] = 0;
                }
            }
        }
        int *newValues = new int[new_cap];
        for (int i = 0; i < nodeCount; i++)
        {
            newValues[i] = nodeValues[i];
        }
        for (int i = 0; i < capacity; i++)
        {
            delete[] matrixGraph[i];
        }
        delete[] matrixGraph;
        delete[] nodeValues;
        matrixGraph = newMatrix;
        nodeValues = newValues;
        capacity = new_cap;
    }

public:
    AdjacencyMatrixGraph()
    {
        nodeCount = 0;
        capacity = 100;
        matrixGraph = new int *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            matrixGraph[i] = new int[capacity];
            for (int j = 0; j < capacity; j++)
            {
                matrixGraph[i][j] = 0;
            }
        }
        nodeValues = new int[capacity];
    }

    ~AdjacencyMatrixGraph()
    {
        for (int i = 0; i < capacity; i++)
        {
            delete[] matrixGraph[i];
        }
        delete[] matrixGraph;
        delete[] nodeValues;
    }

    void AddNode(int v) override
    {
        // TODO: Add a new node v and resize the matrix if your current matrix is almost going to be full.
        if (findNodeIndex(v) != -1)
            return;
        resizeMatrix();
        nodeValues[nodeCount++] = v;
    }

    void AddEdge(int u, int v) override
    {
        // TODO: Add a new edge between the nodes u and v
        if (findNodeIndex(u) == -1)
        {
            AddNode(u);
        }
        if (findNodeIndex(v) == -1)
        {
            AddNode(v);
        }
        int i = findNodeIndex(u);
        int j = findNodeIndex(v);
        matrixGraph[i][j] = 1;
        matrixGraph[j][i] = 1;
    }

    bool CheckEdge(int u, int v) const override
    {
        // TODO: Check whether there is an edge between two nodes u and v
        int i = findNodeIndex(u);
        int j = findNodeIndex(v);
        if (i == -1 || j == -1)
            return false;
        return matrixGraph[i][j] == 1;
    }

    void RemoveNode(int v) override
    {
        // TODO: Remove a node.
        int v_index = findNodeIndex(v);
        if (v_index == -1)
            return;

        for (int i = v_index; i < nodeCount - 1; i++)
        {
            nodeValues[i] = nodeValues[i + 1];
        }

        for (int i = 0; i < nodeCount; i++)
        {
            for (int j = v_index; j < nodeCount - 1; j++)
            {
                matrixGraph[i][j] = matrixGraph[i][j + 1];
            }
        }
        for (int i = v_index; i < nodeCount - 1; i++)
        {
            for (int j = 0; j < nodeCount - 1; j++)
            {
                matrixGraph[i][j] = matrixGraph[i + 1][j];
            }
        }
        nodeCount--;
    }

    void RemoveEdge(int u, int v) override
    {
        // TODO: remove an edge
        int i = findNodeIndex(u);
        int j = findNodeIndex(v);
        if (i == -1 || j == -1)
            return;
        matrixGraph[i][j] = 0;
        matrixGraph[j][i] = 0;
    }

    bool CheckPath(int u, int v) const override
    {
        // TODO: Return true if there is a path between nodes u and v. Otherwise return false
        int u_index = findNodeIndex(u);
        int v_index = findNodeIndex(v);
        if (u_index == -1 || v_index == -1)
            return false;

        bool *visited = new bool[nodeCount]{false};
        ListQueue q;
        q.enqueue(u);
        visited[u_index] = true;

        while (!q.empty())
        {
            int curr = q.dequeue();
            int curr_index = findNodeIndex(curr);
            if (curr == v)
            {
                delete[] visited;
                return true;
            }
            for (int i = 0; i < nodeCount; i++)
            {
                if (matrixGraph[curr_index][i] && !visited[i])
                {
                    q.enqueue(nodeValues[i]);
                    visited[i] = true;
                }
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
        bool *visited = new bool[nodeCount]{false};
        int *prev = new int[nodeCount];
        for (int i = 0; i < nodeCount; i++)
        {
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

            for (int i = 0; i < nodeCount; i++)
            {
                if (matrixGraph[curr_index][i] && !visited[i])
                {
                    visited[i] = true;
                    prev[i] = curr_index;
                    q.enqueue(nodeValues[i]);
                    if (nodeValues[i] == v)
                    {
                        found = true;
                        break;
                    }
                }
            }
            if (found)
                break;
        }
        if (!found)
        {
            std::cout << "No path\n";
            delete[] visited;
            delete[] prev;
            return;
        }
        int path[100];
        int len = 0;
        int curr_index = findNodeIndex(v);
        while (curr_index != -1)
        {
            path[len++] = nodeValues[curr_index];
            curr_index = prev[curr_index];
        }
        std::cout << "Shortest path: ";
        for (int i = len - 1; i >= 0; i--)
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
        {
            return -1;
        }

        bool *visited = new bool[nodeCount]{false};
        int *dist = new int[nodeCount];
        for (int i = 0; i < nodeCount; i++)
        {
            dist[i] = -1;
        }
        ListQueue q;
        q.enqueue(u);
        visited[u_index] = true;
        dist[u_index] = 0;
        while (!q.empty())
        {
            int curr = q.dequeue();
            int curr_index = findNodeIndex(curr);
            for (int i = 0; i < nodeCount; i++)
            {
                if (matrixGraph[curr_index][i] && !visited[i])
                {
                    visited[i] = true;
                    dist[i] = dist[curr_index] + 1;
                    q.enqueue(nodeValues[i]);
                    if (nodeValues[i] == v)
                    {
                        int result = dist[i];
                        delete[] visited;
                        delete[] dist;
                        return result;
                    }
                }
            }
        }
        delete[] visited;
        delete[] dist;
        return -1;
    }

    linkedList GetNeighbors(int u) const override
    {
        // TODO return a list of neighbors of node u
        linkedList neighbors;
        init(&neighbors);
        int u_index = findNodeIndex(u);
        if (u_index == -1)
        {
            return neighbors;
        }
        for (int i = 0; i < nodeCount; i++)
        {
            if (matrixGraph[u_index][i]==1)
            {
                append(nodeValues[i], &neighbors);
            }
        }
        return neighbors;
    }
};

#endif // ADJACENCY_MATRIX_GRAPH_H
