#ifndef OOP8_TREE4_H
#define OOP8_TREE4_H
#include <iostream>
using namespace std;

template <typename T>
class Node {
public:
    T keys[3];
    Node<T>* children[4];
    bool leaf;
    int keyCount;

    Node(bool leaf) : leaf(leaf), keyCount(0) {
        for (int i = 0; i < 4; i++)
            children[i] = nullptr;
    }

    bool isFull() {
        return keyCount == 3;
    }

    void insertIntoLeaf(T key) {
        int i = keyCount - 1;

        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = key;
        keyCount++;
    }

    void splitChild(int i) {
        Node<T>* child = children[i];
        Node<T>* newNode = new Node<T>(child->leaf);

        T middle = child->keys[1];

        newNode->keys[0] = child->keys[2];
        newNode->keyCount = 1;

        child->keyCount = 1;

        if (!child->leaf) {
            newNode->children[0] = child->children[2];
            newNode->children[1] = child->children[3];
        }

        for (int j = keyCount; j >= i + 1; j--) {
            children[j + 1] = children[j];
        }

        children[i + 1] = newNode;

        for (int j = keyCount - 1; j >= i; j--) {
            keys[j + 1] = keys[j];
        }

        keys[i] = middle;
        keyCount++;
    }

    void insertNonFull(T key) {
        int i = keyCount - 1;

        if (leaf) {
            insertIntoLeaf(key);
            return;
        }

        while (i >= 0 && key < keys[i]) {
            i--;
        }
        i++;

        if (children[i]->isFull()) {
            splitChild(i);

            if (key > keys[i]) {
                i++;
            }
        }

        children[i]->insertNonFull(key);
    }

    void print(int level = 0) {
        for (int i = 0; i < level; i++) cout << "    ";

        cout << "[ ";
        for (int i = 0; i < keyCount; i++) {
            cout << keys[i] << " ";
        }
        cout << "]\n";

        if (!leaf) {
            for (int i = 0; i <= keyCount; i++) {
                if (children[i])
                    children[i]->print(level + 1);
            }
        }
    }

    bool search(const T& key) {
        int i = 0;

        while (i < keyCount && key > keys[i]) {
            i++;
        }

        if (i < keyCount && keys[i] == key) {
            return true;
        }

        if (leaf) {
            return false;
        }

        return children[i]->search(key);
    }
};

template <typename T>
class Tree234 {
private:
    Node<T>* root;

public:
    Tree234() {
        root = new Node<T>(true);
    }

    void insert(T key) {
        if (root->isFull()) {
            Node<T>* newRoot = new Node<T>(false);
            newRoot->children[0] = root;

            newRoot->splitChild(0);
            root = newRoot;
        }

        root->insertNonFull(key);
    }

    void print() {
        if (root)
            root->print();
    }

    bool search(const T& key) {
        if (!root) return false;
        return root->search(key);
    }
};

#endif //OOP8_TREE4_H
