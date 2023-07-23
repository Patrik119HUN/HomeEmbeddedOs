#ifndef DigitalOutput_h
#define DigitalOutput_h

#include <Arduino.h>

#include "../../FileSystem/IFile.h"
class DigitalOutput : public IFile {
   private:
    char _file_name[15] = "Digital_Output";
    uint8_t pins[8] = {PC3, PC2, PF1, PF0, PC15, PC14, PC13, PB7};

   public:
    enum DOCMD { SET_ALL, RESET_ALL };
    DigitalOutput() {
        for (size_t i = 0; i < 8; i++) {
            pinMode(pins[i], OUTPUT);
        }
        setTimeout(0);
    }
    int ioctl(int code, int var) {
        switch (code) {
            case SET_ALL:
                this->write(0b00000000);
                break;
            case RESET_ALL:
                this->write(0b11111111);
                break;
            default:
                break;
        }
        return 1;
    }
    int available() override { return 0; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    int read() override { return EOF; }
    void flush() override { return; };
    size_t write(const uint8_t data) override {
        for (size_t i = 0; i < 8; i++) {
            digitalWrite(pins[i], bitRead((int)data, i));
        }
        return ENOSPC;
    }
    size_t write(const uint8_t* buffer, size_t size) override {
        return ENOSPC;
    }
    char* name() override { return _file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }
};

#endif  // DigitalOutput