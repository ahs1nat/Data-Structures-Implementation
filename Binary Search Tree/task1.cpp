#include <iostream>
#include <fstream>
#include "listBST.hpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Usage: filename" << "\n";
        return 1;
    }
    ifstream in_file(argv[1]);
    if (!in_file)
    {
        cerr << "Unable to open file\n";
        return 2;
    }
    char c, str[5];
    int val;
    BST<int, int> *bst = new ListBST<int, int>();
    if (!bst)
    {
        cerr << "Memory allocation failed\n";
        return 3;
    }
    while (in_file >> c)
    {
        // TODO: Implement the logic to read commands from the file and output accordingly
        // After every insertion and removal, print the BST in nested parentheses format
        // Handle exceptions where necessary and print appropriate error messages

        // Start your code here
        switch (c)
        {
        case 'I':
            in_file >> val;
            if (bst->insert(val, val))
            {
                cout << "Key " << val << " inserted into BST, ";
                bst->print();
                cout << endl;
            }
            else
            {
                cout << "Insertion failed! Key " << val << " already exists in BST, ";
                bst->print();
                cout << endl;
            }
            break;
        case 'F':
            in_file >> val;
            if (bst->find(val))
            {
                cout << "Key " << val << " found in BST." << endl;
            }
            else
            {
                cout << "Key " << val << " not found in BST." << endl;
            }
            break;
        case 'E':
            if (bst->empty())
                cout << "Empty" << endl;
            else
                cout << "Not empty" << endl;
            break;
        case 'M':
            in_file >> c;
            if (c == 'M')
            {
                in_file >> c;
                if (c == 'i')
                {
                    try
                    {
                        cout << "Minimum value: " << bst->find_min() << endl;
                    }
                    catch (const std::out_of_range &e)
                    {
                        cout << "Error: " << e.what() << endl;
                    }
                }
                else
                {
                    try
                    {
                        cout << "Maximum value: " << bst->find_max() << endl;
                    }
                    catch (const std::out_of_range &e)
                    {
                        cout << "Error: " << e.what() << endl;
                    }
                }
            }
            break;
        case 'D':
            in_file >> val;
            if (bst->remove(val))
            {
                cout << "Key " << val << " removed from BST, ";
                bst->print();
                cout << endl;
            }
            else
            {
                cout << "Removal failed! Key " << val << " not found in BST, ";
                bst->print();
                cout << endl;
            }
            break;
        case 'T':
            in_file >> c;
            if (c == 'I')
            {
                bst->print('I');
                //cout << endl;
            }
            else if (c == 'P')
            {
                in_file >> c;
                if (c == 'r')
                {
                    bst->print('P');
                    //cout << endl;
                }
                else if (c == 'o')
                {
                    bst->print('O');
                }
            }
            break;
        case 'S':
            cout << "Size: " << bst->size() << endl;
            break;
        }

        // End your code here
    }
    in_file.close();
    delete bst;
    return 0;
}
