#pragma once

#include <Arduino.h>
#include <file_interface.h>
class terminal : public IFile {
   public:
    terminal() {
        m_name = "terminal";
        setTimeout(0);
    }
    bool seek(uint32_t pos) override { return false; }
    int available() override { return Serial.available(); }
    int peek() override { return EOF; }
    int read() override { return EOF; }
    void flush() override { return; };

    size_t write(const uint8_t data) override { return Serial.write(data); }
    size_t write(const uint8_t* buffer, size_t size) override { return Serial.write(buffer, size); }

    int ioctl(int code, int var) override { return 1; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }

   private:
};