#ifndef Device_h
#define Device_h

#include <Arduino.h>

class Device : public Stream {
   public:
    Device(){};
    ~Device(){};

    int available();

    int seek(int pos);
    int peek();
    
    void flush();

    size_t write(const uint8_t data);
    size_t write(const uint8_t* buffer, size_t size);

    int read();
    int ioctl(int code, int var);
};

#endif  // Device