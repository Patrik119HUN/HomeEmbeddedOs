#ifndef full_h
#define full_h
#include <Arduino.h>

#include "../../Utils/Device/Device.h"

class full : public Device {
   public:
    full() { setTimeout(0); }
    int available() { return 0; }
    int peek() { return EOF; }
    int read() { return EOF; }
    void flush() { return; };

    size_t write(const uint8_t data) { return ENOSPC; }
    size_t write(const uint8_t* buffer, size_t size) { return ENOSPC; }
    int ioctl(int code, int var) { return 1; }
};
#endif  // full