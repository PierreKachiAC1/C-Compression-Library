#include <bits/stdc++.h>

using namespace std;

#ifndef COMPRESSION_LIBRARY_BINARYSTDIN_H
#define COMPRESSION_LIBRARY_BINARYSTDIN_H
class BinaryStdIn {
private:
    string file;
    char buffer;
    ifstream in;
    char n;
    bool isInitialized;

    void initialize() {
        in = ifstream(file, std::ios::binary);
        buffer = 0;
        n = 0; // remaining bits
        fillBuffer();
        isInitialized = true;
    }

    void fillBuffer() {
        try {
            in.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
            n = 8;
        }
        catch (exception &e) {
            buffer = EOF;
            n = -1;
        }
    }

public:
    explicit BinaryStdIn(string && file) : file(std::move(file)) {
        initialize();
    }

    void close() {
        if (!isInitialized) initialize();

        try {
            in.close();
            isInitialized = false;
            std::cout << "\n";
        }
        catch (exception& ioe) {
            std::cout << "Could not close file";
        }
    }

    bool readBoolean() {
        if (isEmpty()) throw std::exception();
        n--;
        bool bit = ((buffer >> n) & 1) == 1; // take the bit
        if (n == 0) fillBuffer();
        return bit;
    }

    unsigned char readChar() {
        if(!isInitialized) initialize();
        if (isEmpty()) throw exception();

        // special case when aligned byte
        if (n == 8) {
            unsigned char x = buffer;
            fillBuffer();
            return x;
        }

        // combine last n bits of current buffer with first 8-n bits of new buffer
        unsigned char x = buffer;
        x <<= (8 - n);
        char oldN = n;
        fillBuffer();
        if (isEmpty()) throw exception();
        n = oldN;
        x |= (static_cast<unsigned char>(buffer) >> n);
        return x;
        // the above code doesn't quite work for the last character if n = 8
        // because buffer will be -1, so there is a special case for aligned byte

    }

    int readInt() {
        int x = 0;
        for (int i = 0; i < 4; i++) {
            unsigned char c = readChar();
            x <<= 8;
            x |= c;
        }
        return x;
    }

    int readInt(int r) {
        if (r < 1 || r > 32) throw exception();

        // optimize r = 32 case
        if (r == 32) return readInt();

        int x = 0;
        for (int i = 0; i < r; i++) {
            x <<= 1;
            bool bit = readBoolean();
            if (bit) x |= 1;
        }
        return x;
    }

    string readString() {
        stringstream ss;
        if (isEmpty()) throw exception();

        while (!isEmpty()) {
            unsigned char c = readChar();
            ss << c;
        }
        return ss.str();
    }

    bool isEmpty() {
        if (!isInitialized) initialize();
        return in.eof(); // && buffer == EOF;
    }
};
#endif //COMPRESSION_LIBRARY_BINARYSTDIN_H
