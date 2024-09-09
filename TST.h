#include <bits/stdc++.h>
#include "BinaryStdIn.h"
#include "BinaryStdOut.h"

using namespace std;

#ifndef COMPRESSION_LIBRARY_TST_H
#define COMPRESSION_LIBRARY_TST_H
class TST
{
private:
    class Node
    {
    public:
        char c; // character
        Node * left = nullptr;
        Node * mid = nullptr;
        Node * right = nullptr;
        int val = -1; // value associated with string
    };

    Node* root; // root of trie

    Node* get(Node * x, const string & key, int d)
    {
        if (x == nullptr) return nullptr;
        char c = key[d];
        if (c < x->c) return get(x->left, key, d);
        else if (c > x->c) return get(x->right, key, d);
        else if (d < key.length() - 1)
            return get(x->mid, key, d+1);
        else return x;
    }

    Node* put(Node * x, const string & key, int val, int d)
    {
        char c = key[d];
        if (x == nullptr) { x = new Node(); x->c = c; }
        if (c < x->c) x->left = put(x->left, key, val, d);
        else if (c > x->c) x->right = put(x->right, key, val, d);
        else if (d < key.length() - 1)
            x->mid = put(x->mid, key, val, d+1);
        else x->val = val;
        return x;
    }



    int search(Node * node, const string & query, int digit, int length) {
        if (node == nullptr) {
            return length;
        }
        if (node->val != -1 && node->c == query[digit]) {
            length = digit + 1;
        }
        char nextChar = query[digit];

        if (nextChar < node->c) {
            return search(node->left, query, digit, length);
        } else if (nextChar > node->c) {
            return search(node->right, query, digit, length);
        } else if (digit < query.length() - 1) {
            return search(node->mid, query, digit + 1, length);
        } else {
            return length;
        }
    }

    void collect(Node* node, stringstream& prefix, vector<string>& queue) {
        if (node == nullptr) {
            return;
        }
        collect(node->left, prefix, queue);

        if (node->val != -1) {
            queue.push_back(prefix.str() + node->c);
        }

        prefix << node->c;
        collect(node->mid, prefix, queue);
        string s = prefix.str();
        s.erase(s.length() - 1);
        prefix.clear();
        prefix << s;
        collect(node->right, prefix, queue);
    }

public:
    TST() {
        root = nullptr;
    }

    vector<string> keys() {
        vector<string> keys;
        stringstream ss;
        collect(root, ss, keys);
        return keys;
    }


    int  get(std::string & key) {
        Node* x = get(root, key, 0);
        if (x == nullptr) return -1;
        return (int) x->val;
    }// same as for tries (See page 737).

    void put(const string & key, int val)
    { root = put(root, key, val, 0); }

    std::string longestPrefixOf(std::string & s)
    {
        int length = search(root, s, 0, 0);
//        return s.substring(0, length);
        return s.substr(0, length);
    }
};
#endif //COMPRESSION_LIBRARY_TST_H
