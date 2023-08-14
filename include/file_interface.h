#pragma once

#include <Arduino.h>

class IFile : public Stream {
   public:
    virtual int available();

    virtual bool seek(uint32_t);
    virtual int peek();

    virtual int read();
    virtual void flush();

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t*, size_t);

    virtual int ioctl(int, int) = 0;
    virtual char* name() = 0;
    virtual bool isDirectory(void) = 0;

    virtual void close() = 0;
};