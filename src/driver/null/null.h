#pragma once

#include <Arduino.h>
#include <file_interface.h>
class null : public IFile {
  public:
    null() {
        m_name="null";
        setTimeout(0);
        nullVar = -1;
    }
    int available() override { return 0; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return EOF; }

    void flush() override { return; };

    size_t write(const uint8_t data) override {
        this->nullVar = data;
        return 1;
    }
    size_t write(const uint8_t* buffer, size_t size) override {
        if (size > 0) {
            nullVar = buffer[size - 1];
        }
        return size;
    }

    int lastByte() { return nullVar; }
    int ioctl(int code, int var) override { return 1; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }

  private:
    uint8_t nullVar;
};