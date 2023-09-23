#pragma once
#define ENOSPC 28
#include <Arduino.h>

class IFile : public  Stream {
   public:
    IFile() {}
    virtual ~IFile() {}

    virtual int available() = 0;

    virtual bool seek(uint32_t) = 0;
    virtual int peek() = 0;

    virtual int read() = 0;
    virtual void flush()= 0;

    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t*, size_t) =0;

    virtual int ioctl(int, int) = 0;
    virtual char* name() = 0;
    virtual bool isDirectory(void) = 0;

    virtual void close() = 0;
};