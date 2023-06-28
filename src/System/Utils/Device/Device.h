#ifndef Device_h
#define Device_h

#include <Arduino.h>

class Device {
   public:
    Device(){};
    ~Device(){};
    int seek(int pos);
    int read();
    int write(int str);
    int ioctl(int code, int var);
};

#endif  // Device