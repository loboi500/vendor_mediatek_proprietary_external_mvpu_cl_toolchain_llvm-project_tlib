// SPDX-License-Identifier: Apache-2.0

#ifndef CL_COMPILER_READWRITESTREAM_H
#define CL_COMPILER_READWRITESTREAM_H

#include "GPUUtil.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>

namespace cl_compiler
{

class ReadWriteStream
{
public:
    virtual ~ReadWriteStream() {};
    virtual bool isReadStream() = 0;
    virtual bool isMapping() = 0;
    virtual const char *getPtr() = 0;
    virtual unsigned int getSize() = 0;
    virtual unsigned int padding(void *data, unsigned int bytes) = 0;
    virtual unsigned int serialize(void *data, unsigned int bytes = 0, bool isPtr = false) = 0;
};

using AllocFuncTy = std::function<void *(std::size_t)>;

class ReadStream : public ReadWriteStream
{
private:
    FILE *file = nullptr;
    AllocFuncTy allocFunc = nullptr;
    const char *buf = nullptr;
    unsigned int bufSize = 0;
    unsigned int offset = 0;
    bool mapping = false;

public:
    ReadStream(FILE *file, AllocFuncTy allocCallBack = &std::malloc)
        : file(file), allocFunc(std::move(allocCallBack))
    {
        assert(file);
    }

    ReadStream(const char *buf, unsigned int bufSize, AllocFuncTy allocCallBack = &std::malloc, bool mapping = false)
        : allocFunc(std::move(allocCallBack)), buf(buf), bufSize(bufSize), mapping(mapping)
    {
        assert(buf);
    }

    ReadStream(const ReadStream &) = delete;

    ReadStream(ReadStream &&other)
        : file(other.file),
          allocFunc(std::move(other.allocFunc)),
          buf(other.buf),
          bufSize(other.bufSize),
          offset(other.offset),
          mapping(other.mapping)
    {
        other.file = nullptr;
        other.buf = nullptr;
        other.bufSize = 0;
        other.offset = 0;
        other.mapping = false;
    }

    ~ReadStream() {}

    bool isReadStream() override { return true; }
    bool isMapping() override { return mapping; }
    const char *getPtr() override { return buf; }
    unsigned int getSize() override { return bufSize; }

    AllocFuncTy setAllocFunc(AllocFuncTy alloc)
    {
        AllocFuncTy old = std::move(allocFunc);
        allocFunc = std::move(alloc);
        return old;
    }

    unsigned int read(unsigned int bytes, void *data)
    {
        if (file)
        {
            size_t result = fread(data, bytes, 1, file);
            if (result != 1)
                return 0; // FIXME: What should we return if this case happens?
        }
        else
        {
            memcpy(data, &buf[offset], bytes);
            offset += bytes;
        }
        return bytes;
    }

    unsigned int padding(void *data, unsigned int bytes) override
    {
        return read(bytes, data);
    }

    unsigned int serialize(void *data, unsigned int bytes = 0, bool isPtr = false) override
    {
        unsigned int count = 0;
        unsigned char flag;
        count += read(sizeof(flag), &flag);
        if (flag == 3) // Is a nullptr pointer
        {
            *(void **)data = nullptr;
            return count;
        }
        // coverity[tainted_argument]
        count += read(sizeof(bytes), &bytes);
        if (flag & 0x1)
        {
            if (buf && mapping)
            {
                *(void **)data = const_cast<char *>(&buf[offset]);
                offset += bytes;
            }
            else
            {
                void *ptr = (bytes == 0) ? nullptr : allocFunc(bytes);
                if (bytes)
                    count += read(bytes, ptr);
                *(void **)data = ptr;
            }
        }
        else
            count += read(bytes, data);
        return count;
    }
};

class WriteStream : public ReadWriteStream
{
private:
    FILE *file = nullptr;
    AllocFuncTy allocFunc;
    std::unique_ptr<std::vector<char>> buf;

public:
    WriteStream(FILE *file, AllocFuncTy allocCallBack = &std::malloc)
        : file(file), allocFunc(std::move(allocCallBack))
    {
        if (file)
            return;
        buf.reset(new std::vector<char>());
    }

    WriteStream(const WriteStream &) = delete;

    WriteStream(WriteStream &&other)
        : file(other.file),
          allocFunc(std::move(other.allocFunc)),
          buf(std::move(other.buf))
    {
        other.file = nullptr;
    }

    ~WriteStream() {}

    bool isReadStream() override { return false; }
    bool isMapping() override { return false; }
    const char *getPtr() override { return (buf == 0 || buf->size() == 0) ? 0 : &(*buf)[0]; }
    unsigned int getSize() override { return (buf == 0) ? 0 : buf->size(); }

    unsigned int write(unsigned int bytes, void *data)
    {
        if (file)
            fwrite(data, bytes, 1, file);
        else
        {
            unsigned int size = buf->size();
            buf->resize(size + bytes);
            memcpy(&(*buf)[size], data, bytes);
        }
        return bytes;
    }

    char *flush(unsigned int *bytes)
    {
        if (buf == 0 || buf->size() == 0)
            return 0;
        unsigned int size = buf->size();
        void *ptr = allocFunc(size);
        if (ptr)
        {
            memcpy(ptr, &(*buf)[0], size);
            *bytes = size;
        }
        return (char *)ptr;
    }

    unsigned int padding(void *data, unsigned int bytes) override
    {
        return write(bytes, data);
    }

    unsigned int serialize(void *data, unsigned int bytes = 0, bool isPtr = false) override
    {
        unsigned int count = 0;
        unsigned char flag = (isPtr) ? 0x1 : 0;
        void *ptr = (isPtr) ? *(void **)data : data;
        if (bytes == 0)
        {
            assert(isPtr); // Must be a pointer
            flag |= 0x2; // Be a null pointer
        }
        count += write(sizeof(flag), &flag);
        if (bytes)
        {
            count += write(sizeof(bytes), &bytes);
            count += write(bytes, ptr);
        }
        return count;
    }
};

} // namespace cl_compiler

#endif // ! CL_COMPILER_READWRITESTREAM_H
