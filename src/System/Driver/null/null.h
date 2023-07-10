#ifndef null_h
#define null_h

#include <Arduino.h>

#include "../../Utils/Device/Device.h"

class null : public Device {
   private:
    uint8_t nullVar;

   public:
    null() {
        setTimeout(0);
        nullVar = -1;
    }
    ~null();
    int available() { return 0; }
    int peek() { return EOF; }
    int read() { return EOF; }
    void flush() { return; };

    size_t write(const uint8_t data) {
        this->nullVar = data;
        return 1;
    }
    size_t write(const uint8_t* buffer, size_t size) {
        if (size > 0) {
            nullVar = buffer[size - 1];
        }
        return size;
    }

    int lastByte() { return nullVar; }
};

#endif  // null