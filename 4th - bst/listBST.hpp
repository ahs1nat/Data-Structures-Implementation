#ifndef LISTBST_H
#define LISTBST_H

#include "BST.hpp"
#include <iostream>
#include <stdexcept>

/**
 * Binary Search Tree implementation using linked list structure
 *
 * @tparam Key - The type of keys stored in the BST
 * @tparam Value - The type of values associated with keys
 */
template <typename Key, typename Value>
class ListBST : public BST<Key, Value>
{
private:
    /**
     * Node class for the binary search tree
     */
    class Node
    {
    public:
        Key key;
        Value value;
        Node *left;
        Node *right;

        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node *root;
    size_t node_count;

    // TODO: Implement private helper functions as needed
    // Start your private helper functions here
    Node *insertNode(Node *root, Key k, Value v)
    {
        if (root == nullptr)
        {
            root = new Node(k, v);
            node_count++;
        }
        else if (k < root->key)
        {
            root->left = insertNode(root->left, k, v);
        }
        else if (k > root->key)
        {
            root->right = insertNode(root->right, k, v);
        }
        return root;
    }

    Node *findMinNode(Node *node) const
    {
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    Node *removeNode(Node *root, Key k)
    {
        if (root == nullptr)
            return root;
        else if (k < root->key)
        {
            root->left = removeNode(root->left, k);
        }
        else if (k > root->key)
        {
            root->right = removeNode(root->right, k);
        }
        else
        {
            if (root->left == nullptr)
            {
                Node *temp = root->right;
                delete root;
                node_count--;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node *temp = root->left;
                delete root;
                node_count--;
                return temp;
            }
            else
            {
                Node *temp = findMinNode(root->right);
                root->key = temp->key;
                root->value = temp->value;
                root->right = removeNode(root->right, temp->key);
            }
        }
        return root;
    }

    Node *findNode(Node *root, Key k) const
    {
        if (root == nullptr || root->key == k)
            return root;
        if (k < root->key)
            return findNode(root->left, k);
        return findNode(root->right, k);
    }

    void clearNodes(Node *root)
    {
        if (root)
        {
            clearNodes(root->left);
            clearNodes(root->right);
            delete root;
        }
    }

    void print_default(Node *root) const
    {
        if (root == nullptr)
        {
            return;
        }
        std::cout << " (" << root->key << ":" << root->value;
        if (root->left != nullptr || root->right != nullptr)
        {
            if (root->left != nullptr)
            {
                print_default(root->left);
            }
            else
            {
                std::cout << " ()";
            }
        }
        print_default(root->right);
        std::cout << ")";
    }

    void print_inorder(Node *root) const
    {
        if(root==nullptr)
        {
            return;
        }
        print_inorder(root->left);
        std::cout << " (" << root->key << ":" << root->value << ")";
        print_inorder(root->right);
    }

    void print_preorder(Node *root) const
    {
        if (root == nullptr)
        {
            return;
        }
        std::cout << " (" << root->key << ":" << root->value << ")";
        print_preorder(root->left);
        print_preorder(root->right);
    }

    void print_postorder(Node *root) const
    {
        if (root == nullptr)
        {
            return;
        }
        print_postorder(root->left);
        print_postorder(root->right);
        std::cout << " (" << root->key << ":" << root->value << ")";
    }

    // End your private helper functions here

public:
    /**
     * Constructor
     */
    ListBST() : root(nullptr), node_count(0) {}

    /**
     * Destructor
     */
    ~ListBST()
    {
        // TODO: Implement destructor to free memory
        clear();
    }

    /**
     * Insert a key-value pair into the BST
     */
    bool insert(Key key, Value value) override
    {
        // TODO: Implement insertion logic
        size_t oldsize = node_count;
        root = insertNode(root, key, value);
        return oldsize < node_count;
    }

    /**
     * Remove a key-value pair from the BST
     */
    bool remove(Key key) override
    {
        // TODO: Implement removal logic
        size_t oldsize = node_count;
        root = removeNode(root, key);
        return oldsize > node_count;
    }

    /**
     * Find if a key exists in the BST
     */
    bool find(Key key) const override
    {
        // TODO: Implement find logic
        Node *node = findNode(root, key);
        if (node == nullptr)
            return false;
        return true;
    }

    /**
     * Find a value associated with a given key
     */
    Value get(Key key) const override
    {
        // TODO: Implement get logic
        Node *node = findNode(root, key);
        if (node != nullptr)
            return node->value;
        else
        {
            throw std::out_of_range("Key not found");
        }
    }

    /**
     * Update the value associated with a given key
     */
    void update(Key key, Value value) override
    {
        // TODO: Implement update logic
        Node *node = findNode(root, key);
        if (node == nullptr)
        {
            throw std::out_of_range("Key not found");
        }
        node->value = value;
    }

    /**
     * Clear all elements from the BST
     */
    void clear() override
    {
        // TODO: Implement clear logic
        clearNodes(root);
        root = nullptr;
        node_count = 0;
    }

    /**
     * Get the number of keys in the BST
     */
    size_t size() const override
    {
        // TODO: Implement size logic
        return node_count;
    }

    /**
     * Check if the BST is empty
     */
    bool empty() const override
    {
        // TODO: Implement empty check logic
        return node_count == 0;
    }

    /**
     * Find the minimum key in the BST
     */
    Key find_min() const override
    {
        // TODO: Implement find_min logic
        if (empty())
        {
            throw std::out_of_range("Tree is empty");
        }
        Node *node = findMinNode(root);
        return node->key;
    }

    /**
     * Find the maximum key in the BST
     */
    Key find_max() const override
    {
        // TODO: Implement find_max logic
        if (empty())
        {
            throw std::out_of_range("Tree is empty");
        }
        Node *temp = root;
        while (temp != nullptr && temp->right != nullptr)
        {
            temp = temp->right;
        }
        return temp->key;
    }

    /**
     * Print the BST using specified traversal method
     */
    void print(char traversal_type = 'D') const override
    {
        // TODO: Implement print logic
        switch (traversal_type)
        {
        case 'I':
            if (root == nullptr)
                std::cout << "()" << std::endl;
            else
            {
                std::cout << "BST (In-order):";
                print_inorder(root);
                std::cout << std::endl;
            }
            break;
        case 'P':
            if (root == nullptr)
                std::cout << "()" << std::endl;
            else
            {
                std::cout << "BST (Pre-order):";
                print_preorder(root);
                std::cout << std::endl;
            }
            break;
        case 'O':
            if (root == nullptr)
                std::cout << "()" << std::endl;
            else
            {
                std::cout << "BST (Post-order):";
                print_postorder(root);
                std::cout << std::endl;
            }
            break;
        default:
            if (root == nullptr)
                std::cout << "()" << std::endl;
            else
            {
                std::cout << "BST (Default):";
                print_default(root);
            }
        }
    }
};

#endif // LISTBST_H