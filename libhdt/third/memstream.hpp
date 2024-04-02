#ifndef LIBHDT_THIRD_MEMSTREAM_H
#define LIBHDT_THIRD_MEMSTREAM_H

#include <istream>
#include <ostream>
#include <streambuf>
// for EOF:
#include <cstdio>
// for memmove():
#include <cstring>


// low-level read and write functions
#ifdef _MSC_VER
# include <io.h>
#else
# include <unistd.h>
#endif

class memstream : public std::istream {
public:
    memstream(const char* buffer, size_t size)
        : std::istream(&_buffer), _buffer(buffer, size) {
        rdbuf(&_buffer);
    }

private:
    class Buffer : public std::streambuf {
    public:
        Buffer(const char* buffer, size_t size) {
            setg(const_cast<char*>(buffer), const_cast<char*>(buffer), const_cast<char*>(buffer + size));
        }

        std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which) override {
            if (way == std::ios_base::cur) {
                gbump(static_cast<int>(off));
            } else if (way == std::ios_base::end) {
                setg(eback(), egptr() + off, egptr());
            } else if (way == std::ios_base::beg) {
                setg(eback(), eback() + off, egptr());
            }
            return gptr() - eback();
        }

        std::streampos seekpos(std::streampos sp, std::ios_base::openmode which) override {
            return seekoff(sp, std::ios_base::beg, which);
        }
    };

    Buffer _buffer;
};

#endif  // LIBHDT_THIRD_MEMSTREAM_H
