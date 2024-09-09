#include <bits/stdc++.h>
#include "BinaryStdOut.h"
#include "BinaryStdIn.h"

#ifndef COMPRESSION_LIBRARY_HUFFMANENCODING_H
#define COMPRESSION_LIBRARY_HUFFMANENCODING_H

using namespace std;

const int R = 256; // alphabet size (ASCII)

class Huffman {
private:
    BinaryStdIn in;
    BinaryStdOut out;

    class Node { // Huffman trie node
    public:
        unsigned char ch; // unused for internal nodes
        int freq; // unused for expand
        Node * left;
        Node * right;


        Node(unsigned char ch, int freq, Node * left, Node * right)
        {
            this->ch = ch;
            this->freq = freq;
            this->left = left;
            this->right = right;
        }

        [[nodiscard]] bool isLeaf() const {
            return left == nullptr && right == nullptr;
        }

    };

    Node * readTrie() {
        bool bit = in.readBoolean();
        if (bit)
        {
            unsigned char tmp = in.readChar();
            return new Node(tmp, 0, nullptr, nullptr);
        }
        return new Node('\0', 0, readTrie(), readTrie());
    }

    void writeTrie(Node * x) { // Write bitstring-encoded trie.
        if (x->isLeaf())
        {
            out.write(true);
            out.write((unsigned char)x->ch);
            return;
        }

        out.write(false);
        writeTrie(x->left);
        writeTrie(x->right);
    }

    static Node* buildTrie(int freq[]) {
        // Initialize priority queue with singleton trees.
        std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> pq(
                [](Node* a, Node* b) {
                    return a->freq > b->freq;
                }
        );

        for (unsigned char c = 0; c < R - 1; c++)
            if (freq[c] > 0)
                pq.emplace(new Node(c, freq[c], nullptr, nullptr));

        while (pq.size() > 1)
        { // Merge two smallest trees.
            Node * x = pq.top();
            pq.pop();
            Node * y = pq.top();
            pq.pop();
            Node * parent = new Node('\0', x->freq + y->freq, x, y);
            pq.emplace(parent);
        }
        auto min = pq.top();
        pq.pop();
        return min;
    }


    void buildCode(array<string, R> & st, Node* x, const string & s)
    {   // Make a lookup table from trie (recursive).
        if (x->isLeaf()) {
            st[x->ch] = s;
            return;
        }
        buildCode(st, x->left, s + '0');
        buildCode(st, x->right, s + '1');
    }

public:
    explicit Huffman(string && input = "", string && output = "") : in(BinaryStdIn(std::move(input))), out(BinaryStdOut(std::move(output))) {
    }

    void expand(string && source, string && destination)
    {
        //write output
        stringstream ss;
        ofstream output_file = ofstream(destination);
        in = BinaryStdIn(std::move(source));

        Node* root = readTrie();

        int N = in.readInt();

        for (int i = 0; i < N; i++)
        { // Expand ith codeword.
            Node* x = root;
            while (!x->isLeaf())
            {
                if (in.readBoolean())
                    x = x->right;
                else x = x->left;
            }
            ss << x->ch;
        }
        in.close();
        output_file << ss.str();
    }

    void compress(string && source, string && destination)
    {
        stringstream ss;
        ifstream input_file(source);
        out = BinaryStdOut(std::move(destination));

        char tmp;
        while(input_file.read(&tmp, sizeof(tmp)))
        {
            ss << tmp;
        }

        string s = ss.str();

        std::vector<char> input(s.begin(), s.end());
        // Tabulate frequency counts.
        int freq[R]{};
        for (char i : input)
            freq[i]++;
        // Build Huffman code trie.
        Node * root = buildTrie(freq);
        // Build code table (recursive).
        array<string, R> st;
        buildCode(st, root, "");
        // Print trie for decoder (recursive).
        writeTrie(root);
        // Print number of chars.
        out.write((int)input.size());
        // Use Huffman code to encode input.
        for (char i : input)
        {
            std::string code = st[i];
            for (int j = 0; j < code.length(); j++)
                if (code[j] == '1'){
                    out.write(true);
                }
                else{
                    out.write(false);
                }
        }
        out.close();
    }
};
#endif //COMPRESSION_LIBRARY_HUFFMANENCODING_H
