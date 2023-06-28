#ifndef DigitalOutput_h
#define DigitalOutput_h

#include <Arduino.h>

#include "../../Utils/Device/Device.h"
class DigitalOutput : public Device {
   private:
uint8_t pins[8] = {PC3, PC2, PF1, PF0, PC15, PC14, PC13, PB7};
   public:
    enum DOCMD { SET_ALL, RESET_ALL };
    DigitalOutput() {
        for (size_t i = 0; i < 8; i++) {
            pinMode(pins[i], OUTPUT);
        }
    }
    int seek(int pos) { return 1; }
    int read() { return 1; }
    int write(int str) {
        for (size_t i = 0; i < 8; i++) {
            digitalWrite(pins[i], bitRead(str, i));
        }
        return 1;
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
};

#endif  // DigitalOutput