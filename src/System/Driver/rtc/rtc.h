#ifndef rtc_h
#define rtc_h

#include <Arduino.h>
#include "../../Utils/Device/Device.h"
#include <RTClib.h>

class rtc : public Device {
   private:
    RTC_DS3231* RTCInstance = NULL;

   public:
    enum RTCCMD { RTC_SET_TIME, RTC_ALM_READ, RTC_ALM_SET, RTC_AIE_ON, RTC_AIE_OFF };
    rtc() {
        this->RTCInstance = new RTC_DS3231();
        if (!this->RTCInstance->begin()) {
            Serial.println("Couldn't find RTC");
            Serial.flush();
            abort();
        }
    };
    ~rtc() { delete RTCInstance; }
    int seek(int pos) { return 1; }
    int read() { return RTCInstance->unixTime(); }
    int write(int str) { return 1; }
    int ioctl(int code, int var) {
        switch (code) {
            case RTC_SET_TIME:
                break;
            case RTC_ALM_READ:
                break;
            case RTC_ALM_SET:
                break;
            case RTC_AIE_ON:
                break;
            case RTC_AIE_OFF:
                break;
            default:
                break;
        }
        return 1;
    }
};

#endif  // rtc