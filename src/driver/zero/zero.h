#pragma once

#include <Arduino.h>
#include <file_interface.h>

class zero : public IFile {
   public:
    zero() {
        m_name = "zero";
        setTimeout(0);
    }
    int available() override { return 0; }
    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return 0; }
    void flush() override { return; };

    size_t write(const uint8_t data) override { return 1; }
    size_t write(const uint8_t* buffer, size_t size) override { return 1; }
    int ioctl(int code, int var) override { return 1; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }
};