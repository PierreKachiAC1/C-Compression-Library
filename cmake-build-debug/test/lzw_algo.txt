#include <bits/stdc++.h>
#include "BinaryStdIn.h"
#include "BinaryStdOut.h"
#include "TST.h"

using namespace std;

#ifndef COMPRESSION_LIBRARY_LZW_H
#define COMPRESSION_LIBRARY_LZW_H
class LZW {
    int R = 256; // number of input chars
    int L = 4096; // number of codewords = 2^12
    int W = 12; // codeword width

    BinaryStdIn in;
    BinaryStdOut out;
public:
    LZW(string && source, string && destination)
            : in(BinaryStdIn(std::move(source))), out(BinaryStdOut(std::move(destination))) {}

    void compress() {
        std::string input = in.readString();
        TST st;
        for (int i = 0; i < R; i++)
        {
            string s;
            s += (char)i;
            st.put(s, i);
        }
        int code = R + 1; // R is codeword for EOF.
        while (!input.empty()) {
            string s = st.longestPrefixOf(input); // Find max prefix match.
            out.write(st.get(s), W); // Print s's encoding.
            size_t t = s.length();
            if (t < input.length() && code < L) // Add s to symbol table.
            {
                st.put(input.substr(0, t + 1), code++);
            }
            input = input.substr(t); // Scan past s in input.
        }

        out.write(R, W); // Write EOF.
        out.close();
    }

    void expand() {
        std::string st[L];
        int i; // next available codeword value
        for (i = 0; i < R; i++) // Initialize table for chars.
        {
            string s;
            s += (char)i;
            st[i] = s;
        }
        st[i++] = " "; // (unused) lookahead for EOF
        int codeword = in.readInt(W);
        std::string val = st[codeword];
        while (true) {
            out.write(val); // Write current substring.
            codeword = in.readInt(W);
            if (codeword == R) break;
            std::string s = st[codeword]; // Get next codeword.
            if (i == codeword) // If lookahead is invalid,
                s = val + val[0]; // make codeword from last one.
            if (i < L)
                st[i++] = val + s[0]; // Add new entry to code table.
            val = s; // Update current codeword.
        }
        out.close();
    }
};
#endif //COMPRESSION_LIBRARY_LZW_H
