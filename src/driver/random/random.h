#pragma once
#include <Arduino.h>
#include <ArduinoECCX08.h>
#include <file_interface.h>
class frandom : public IFile {
   public:
    frandom() {
        m_name = "random";
        setTimeout(0);
    }
    int available() override { return true; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return ECCX08.random(65535); }

    void flush() override { return; };

    size_t write(const uint8_t data) override { return 0; }
    size_t write(const uint8_t* buffer, size_t size) override { return 0; }

    int ioctl(int code, int var) override { return 1; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }

   private:
    uint32_t m_rand_val;
};