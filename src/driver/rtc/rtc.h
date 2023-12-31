#pragma once

#include "../debug/debug.h"
#include "RTClib.h"
#include <Arduino.h>
#include <file_interface.h>
class rtc : public IFile {
  public:
    enum RTCCMD { RTC_SET_TIME, RTC_ALM_READ, RTC_ALM_SET, RTC_AIE_ON, RTC_AIE_OFF };
    rtc() {
        this->RTCInstance = new RTC_DS3231();
        if (!this->RTCInstance->begin()) {
            Serial.println("Couldn't find RTC");
            m_is_availabe = 0;
        }
        // DEBUG_INFO("RTC init succes");
    };
    ~rtc() { delete RTCInstance; }
    int read() override { return RTCInstance->unixTime(); }
    int ioctl(int code, int var) override {
        switch (code) {
        case RTC_SET_TIME:
            this->RTCInstance->adjust(DateTime(var));
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
    int available() override { return m_is_availabe; }

    bool seek(uint32_t pos) override { return false; }
    int peek() override { return EOF; }
    void flush() override { return; };
    size_t write(const uint8_t data) override { return ENOSPC; }
    size_t write(const uint8_t* buffer, size_t size) override { return ENOSPC; }
    char* name() override { return _file_name; }
    bool isDirectory(void) override { return false; }
    void close() override { return; }

  private:
    RTC_DS3231* RTCInstance = nullptr;
    char _file_name[4] = "rtc";
    int m_is_availabe = 1;
};