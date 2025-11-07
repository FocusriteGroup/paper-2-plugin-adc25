#pragma once

#include <array>
#include <cstdio>

#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#endif


namespace TestTools
{

template<typename T, size_t frameSize>
void stdioTest(typename T::Parameters p)
{

#if defined(_WIN32)
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    T processor{ p };
    std::array<float, frameSize> buffer{};
    size_t readBytes = frameSize;
    while (readBytes == frameSize)
    {
        readBytes = fread(buffer.data (), sizeof(float), frameSize, stdin);
        int ndx = 0;
        for (auto & sample : buffer)
        {
            float out;
            out = processor.process (sample);
            fwrite(&out, sizeof(float), 1, stdout);
            fflush(stdout);
        }
    }
}

} // namespace TestTools