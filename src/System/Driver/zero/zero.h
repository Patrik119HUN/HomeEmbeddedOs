#ifndef zero_h
#define zero_h

#include <Arduino.h>

#include "../../Utils/Device/Device.h"

class zero : public Device {
   public:
    zero() { setTimeout(0); }
    int available() { return 0; }
    int peek() { return EOF; }
    int read() { return 0; }
    void flush() { return; };

    size_t write(const uint8_t data) { return 1; }
    size_t write(const uint8_t* buffer, size_t size) { return 1; }
    int ioctl(int code, int var) { return 1; }
};
#endif  // zero