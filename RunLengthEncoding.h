#include <bits/stdc++.h>
#include "BinaryStdOut.h"
#include "BinaryStdIn.h"

using namespace std;

#ifndef COMPRESSION_LIBRARY_RUNLENGTHENCODING_H
#define COMPRESSION_LIBRARY_RUNLENGTHENCODING_H
class RunLengthEncoding {
private:
    BinaryStdIn in;
    BinaryStdOut out;
public:
    explicit RunLengthEncoding(std::string && source, std::string && destination)
            : in(BinaryStdIn(std::move(source))), out(BinaryStdOut(std::move(destination))) {}

    void expand()
    {
        bool b = false;
        while (!in.isEmpty())
        {
            unsigned char cnt = in.readChar();
            for (int i = 0; i < (int)cnt; i++)
            {
                out.write(b);
            }
            b = !b;
        }
        out.close();
    }

    void compress()
    {
        unsigned char cnt = 0;
        bool b, old = false;
        while (!in.isEmpty())
        {
            b = in.readBoolean();
            if (b != old)
            {
                out.write(cnt);
                cnt = 0;
                old = !old;
            }
            else
            {
                if (cnt == 255)
                {
                    out.write(cnt);
                    cnt = 0;
                    out.write(cnt);
                }
            }
            cnt++;
        }
        out.write(cnt);
        out.close();
    }
};
#endif //COMPRESSION_LIBRARY_RUNLENGTHENCODING_H
