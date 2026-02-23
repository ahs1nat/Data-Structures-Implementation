#include <bits/stdc++.h>
using namespace std;

enum Color
{
    RED,
    BLACK
};

template <typename T>
class RedBlackTree
{
private:
    struct Node
    {
        T key;
        Color color;
        int size;
        Node *left, *right, *parent;
        Node(T k)
            : key(k), color(RED), size(1),
              left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node *root = nullptr;

    int getSize(Node *n) const
    {
        return n ? n->size : 0;
    }

    void updateSize(Node *n)
    {
        if (n)
            n->size = 1 + getSize(n->left) + getSize(n->right);
    }

    void rotateLeft(Node *&root, Node *x)
    {
        Node *y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;
        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;

        y->size = x->size;
        updateSize(x);
    }

    void rotateRight(Node *&root, Node *y)
    {
        Node *x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;

        x->parent = y->parent;
        if (!y->parent)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;

        x->size = y->size;
        updateSize(y);
    }

    void fixInsert(Node *&root, Node *z)
    {
        while (z->parent && z->parent->color == RED)
        {
            Node *gp = z->parent->parent;
            if (z->parent == gp->left) //if parent is grandparent's left child
            {
                Node *y = gp->right; //uncle of inserted node
                if (y && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    gp->color = RED;
                    z = gp; //moving up, trating gp as the new inserted node
                }
                else //if uncle is black
                {
                    if (z == z->parent->right) //slide er case 1(b)
                    {
                        z = z->parent;
                        rotateLeft(root, z);
                    }
                    //this is for both cases 1(a) and 1(b)- z left child and right child
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateRight(root, gp);
                }
            }
            else //if parent is grandparent's right child
            {
                Node *y = gp->left; // uncle
                if (y && y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    gp->color = RED;
                    z = gp;
                }
                else //if uncle is black
                {
                    if (z == z->parent->left) //z jodi parent er left child hoy
                    {
                        z = z->parent;
                        rotateRight(root, z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateLeft(root, gp);
                }
            }
        }
        root->color = BLACK;
    }

    Node *minNode(Node *x)
    {
        while (x->left)
            x = x->left;
        return x;
    }

    void transplant(Node *&root, Node *u, Node *v)
    {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v)
            v->parent = u->parent;
    }

    void fixDelete(Node *&root, Node *x, Node *parent)
    {
        while (x != root && (!x || x->color == BLACK))
        {
            if (x == parent->left)
            {
                Node *w = parent->right;
                if (w && w->color == RED)
                {
                    w->color = BLACK;
                    parent->color = RED;
                    rotateLeft(root, parent);
                    w = parent->right;
                }
                if ((!w || (!w->left || w->left->color == BLACK)) &&
                    (!w || (!w->right || w->right->color == BLACK)))
                {
                    if (w)
                        w->color = RED;
                    x = parent;
                    parent = x->parent;
                }
                else
                {
                    if (!w->right || w->right->color == BLACK)
                    {
                        if (w->left)
                            w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(root, w);
                        w = parent->right;
                    }
                    w->color = parent->color;
                    parent->color = BLACK;
                    if (w->right)
                        w->right->color = BLACK;
                    rotateLeft(root, parent);
                    x = root;
                }
            }
            else
            {
                Node *w = parent->left;
                if (w && w->color == RED)
                {
                    w->color = BLACK;
                    parent->color = RED;
                    rotateRight(root, parent);
                    w = parent->left;
                }
                if ((!w || (!w->left || w->left->color == BLACK)) &&
                    (!w || (!w->right || w->right->color == BLACK)))
                {
                    if (w)
                        w->color = RED;
                    x = parent;
                    parent = x->parent;
                }
                else
                {
                    if (!w->left || w->left->color == BLACK)
                    {
                        if (w->right)
                            w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(root, w);
                        w = parent->left;
                    }
                    w->color = parent->color;
                    parent->color = BLACK;
                    if (w->left)
                        w->left->color = BLACK;
                    rotateRight(root, parent);
                    x = root;
                }
            }
        }
        if (x)
            x->color = BLACK;
    }

    int countLess(Node *node, const T &x) const
    {
        if (!node)
            return 0;
        if (x <= node->key)
            return countLess(node->left, x);
        return getSize(node->left) + 1 + countLess(node->right, x);
    }

public:
    bool insert(const T &key)
    {
        Node *z = new Node(key);
        Node *y = nullptr;
        Node *x = root;

        while (x)
        {
            y = x;
            x->size++;
            if (key < x->key)
                x = x->left;
            else if (key > x->key)
                x = x->right;
            else
            {
                delete z;
                while (y)
                {
                    y->size--; //duplicate key pawar karone size undo kora
                    y = y->parent; //going up
                }
                return false;
            }
        }

        z->parent = y;
        if (!y)
            root = z;
        else if (key < y->key)
            y->left = z;
        else
            y->right = z;

        fixInsert(root, z);
        return true;
    }

    bool remove(const T &key)
    {
        Node *z = root;
        while (z && z->key != key)
            z = (key < z->key ? z->left : z->right);

        if (!z)
            return false;

        Node *tmp = z->parent;
        while (tmp)
        {
            tmp->size--;
            tmp = tmp->parent;
        }

        Node *x = nullptr;
        Node *parentOfX = nullptr;
        Color original = z->color;

        if (!z->left)
        {
            x = z->right;
            parentOfX = z->parent;
            transplant(root, z, z->right);
        }
        else if (!z->right)
        {
            x = z->left;
            parentOfX = z->parent;
            transplant(root, z, z->left);
        }
        else
        {
            Node *s = minNode(z->right);
            original = s->color;
            x = s->right;
            parentOfX = s->parent == z ? s : s->parent;

            transplant(root, s, s->right);
            s->left = z->left;
            s->right = z->right;
            s->parent = z->parent;
            s->color = z->color;

            if (s->left)
                s->left->parent = s;
            if (s->right)
                s->right->parent = s;

            transplant(root, z, s);
            updateSize(s);
        }

        delete z;

        if (original == BLACK)
            fixDelete(root, x, parentOfX);

        return true;
    }
    bool find(const T &key) const
    {
        Node *x = root;
        while (x)
        {
            if (key < x->key)
                x = x->left;
            else if (key > x->key)
                x = x->right;
            else
                return true;
        }
        return false;
    }
    int countLessThan(const T &x) const
    {
        return countLess(root, x);
    }
};

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int N;
    cin >> N;
    RedBlackTree<long long> rbt;
    cout << N << endl;
    for (int i = 0;i<N;i++)
    {
        int ei;
        long long xi;
        cin >> ei >> xi;

        long long ri = 0;
        if (ei == 0)
            ri = rbt.remove(xi)?1:0;
        else if (ei == 1)
            ri = rbt.insert(xi)?1:0;
        else if (ei == 2)
            ri = rbt.find(xi)?1:0;
        else if (ei == 3)
            ri = rbt.countLessThan(xi);

        cout << ei << " " << xi << " " << ri << endl;
    }
}
