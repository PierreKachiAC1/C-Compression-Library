#include <bits/stdc++.h>

using namespace std;

#ifndef COMPRESSION_LIBRARY_BINARYSTDOUT_H
#define COMPRESSION_LIBRARY_BINARYSTDOUT_H
class BinaryStdOut {
private:
    std::string file;
    ofstream out;               // output stream (standard output)
    char buffer;                // 8-bit buffer of bits to write
    char n;                     // number of bits remaining in buffer
    bool isInitialized;         // has BinaryStdOut been called for first time?

    // initialize BinaryStdOut
    void initialize() {
        out = ofstream (file, std::ios::binary);
        buffer = 0;
        n = 0;
        isInitialized = true;
    }

    /**
      * Writes the specified bit to standard output.
      */
    void writeBit(bool bit) {
        if (!isInitialized) initialize();

        // add bit to buffer
        buffer <<= 1;
        if (bit) buffer |= 1;

        // if buffer is full (8 bits), write out as a single byte
        n++;
        if (n == 8) clearBuffer();
    }

    /**
      * Writes the 8-bit byte to standard output.
      */
    void writeByte( unsigned char x) {
        if (!isInitialized) initialize();

        // optimized if byte-aligned
        if (n == 0) {
            try {
                out.write(reinterpret_cast<char*>(&x), sizeof(x));
            }
            catch (exception & e) {
                std::cout << e.what();
            }
            return;
        }

        // otherwise write one bit at a time
        for (int i = 0; i < 8; i++) {
            bool bit = ((x >> (8 - i - 1)) & 1) == 1;
            writeBit(bit);
        }
    }

    // write out any remaining bits in buffer to standard output, padding with 0s
    void clearBuffer() {
        if (!isInitialized) initialize();

        if (n == 0) return;
        if (n > 0) buffer <<= (8 - n);
        try {
            out.write(reinterpret_cast<char*>(&buffer), sizeof(buffer));
        }
        catch (exception & e) {
            std::cout << e.what();
        }
        n = 0;
        buffer = 0;
    }

public:

    explicit BinaryStdOut(std::string file) : file(std::move(file)){
        initialize();
    }

    void flush() {
        clearBuffer();
        try {
            out.flush();
        }
        catch (exception & e) {
            std:: cout << e.what();
        }
    }

    void close() {
        flush();
        try {
            out.close();
            isInitialized = false;
        }
        catch (exception & e) {
            std::cout << e.what();
        }
    }


    void write(bool x) {
        writeBit(x);
    }

    void write(unsigned char x) {
        // if (x < 0 || x >= 256) throw exception();
        writeByte(x);
    }

    void write(int x, int r) {
        if (r == 32) {
            write(x);
            return;
        }
        if (r < 1 || r > 32)        throw exception();
        if (x < 0 || x >= (1 << r)) throw exception();
        for (int i = 0; i < r; i++) {
            bool bit = (static_cast<unsigned char>(x >> (r - i - 1)) & 1) == 1;
            writeBit(bit);
        }
    }

    void write(int x) {
        writeByte((static_cast<unsigned char>(x >> 24)) & 0xff);
        writeByte((static_cast<unsigned char>(x >> 16)) & 0xff);
        writeByte((static_cast<unsigned char>(x >>  8)) & 0xff);
        writeByte((static_cast<unsigned char>(x >>  0)) & 0xff);
    }

    void write(char x, int r) {
        if (r == 8) {
            write(x);
            return;
        }
        if (r < 1 || r > 16) throw exception();
        if (x >= (1 << r))   throw exception();
        for (int i = 0; i < r; i++) {
            bool bit = (static_cast<unsigned char>(x >> (r - i - 1)) & 1) == 1;
            writeBit(bit);
        }
    }

    void write(string s) {
        for (int i = 0; i < s.length(); i++)
            write((unsigned char)s[i]);
    }

    void write(string s, int r) {
        for (int i = 0; i < s.length(); i++)
            write(s[i], r);
    }
};
#endif //COMPRESSION_LIBRARY_BINARYSTDOUT_H
