#include <bits/stdc++.h>
using namespace std;

template <typename K, typename V>
class AVLTree
{
private:
    struct Node
    {
        K key;
        V value;
        Node *left, *right;
        int height;

        Node(K k, V v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };
    Node *root = nullptr;
    int height(Node *n)
    {
        return n ? n->height : 0;
    }
    int balance(Node *n)
    {
        return n ? height(n->left) - height(n->right) : 0;
    }
    void update(Node *n)
    {
        n->height = max(height(n->left), height(n->right)) + 1;
    }
    Node *rotateRight(Node *y)
    {
        Node *x = y->left;
        Node *T2 = x->right;
        x->right = y;
        y->left = T2;
        update(y);
        update(x);
        return x;
    }
    Node *rotateLeft(Node *x)
    {
        Node *y = x->right;
        Node *T2 = y->left;
        y->left = x;
        x->right = T2;
        update(x);
        update(y);
        return y;
    }
    Node *minNode(Node *n)
    {
        while (n->left)
            n = n->left;
        return n;
    }
    Node *insert(Node *node, K key, V value, bool &ok)
    {
        if (!node)
        {
            ok = true;
            return new Node(key, value);
        }
        if (key < node->key)
        {
            node->left = insert(node->left, key, value, ok);
        }
        else if (key > node->key)
        {
            node->right = insert(node->right, key, value, ok);
        }
        else
        {
            ok = false;
            return node;
        }
        update(node);
        int b = balance(node);
        if (b > 1 && key < node->left->key)
        {
            return rotateRight(node);
        }
        if (b < -1 && key > node->right->key)
        {
            return rotateLeft(node);
        }
        if (b > 1 && key > node->left->key)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (b < -1 && key < node->right->key)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    Node *remove(Node *node, K key, bool &ok)
    {
        if (!node)
        {
            ok = false;
            return nullptr;
        }
        if (key < node->key)
            node->left = remove(node->left, key, ok);
        else if (key > node->key)
            node->right = remove(node->right, key, ok);
        else
        {
            ok = true;
            if (!node->left || !node->right)
            {
                Node *temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            Node *temp = minNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = remove(node->right, temp->key, ok);
        }
        update(node);
        int b = balance(node);
        if (b > 1 && balance(node->left) >= 0)
        {
            return rotateRight(node);
        }
        if (b > 1 && balance(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (b < -1 && balance(node->right) <= 0)
        {
            return rotateLeft(node);
        }
        if (b < -1 && balance(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    void preorder(Node *n, bool &first)
    {
        if (!n)
            return;
        if (!first)
            cout << " ";
        cout << n->key;
        first = false;
        preorder(n->left, first);
        preorder(n->right, first);
    }
    void inorder(Node *n, bool &first)
    {
        if (!n)
            return;
        inorder(n->left, first);
        if (!first)
            cout << " ";
        cout << n->key;
        first = false;
        inorder(n->right, first);
    }
    void postorder(Node *n, bool &first)
    {
        if (!n)
            return;
        postorder(n->left, first);
        postorder(n->right, first);
        if (!first)
            cout << " ";
        cout << n->key;
        first = false;
    }
    void levelorder(bool &first)
    {
        if (!root)
            return;
        queue<Node *> q;
        q.push(root);
        while (!q.empty())
        {
            Node *n = q.front();
            q.pop();
            if (!first)
                cout << " ";
            cout << n->key;
            first = false;
            if (n->left)
                q.push(n->left);
            if (n->right)
                q.push(n->right);
        }
    }

public:
    bool insert(K key, V value)
    {
        bool ok = false;
        root = insert(root, key, value, ok);
        return ok;
    }
    bool remove(K key)
    {
        bool ok = false;
        root = remove(root, key, ok);
        return ok;
    }
    void traverse(int type)
    {
        bool first = true;
        if (type == 1)
            preorder(root, first);
        else if (type == 2)
            inorder(root, first);
        else if (type == 3)
            postorder(root, first);
        else if (type == 4)
            levelorder(first);
        cout << endl;
    }
};

int main()
{
    int N;
    cin >> N;
    cout << N << endl;
    AVLTree<int, int> tree;
    while (N--)
    {
        int ei, xi;
        cin >> ei >> xi;
        if (ei == 1)
        {
            cout << "1 " << xi << " " << tree.insert(xi, xi) << endl;
        }
        else if (ei == 0)
        {
            cout << "0 " << xi << " " << tree.remove(xi) << endl;
        }
        else if (ei == 2)
        {
            tree.traverse(xi);
        }
    }
    return 0;
}
