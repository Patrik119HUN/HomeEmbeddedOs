#ifndef _RTCLIB_H_
#define _RTCLIB_H_

#include <Arduino.h>
#include <Wire.h>

#include "const.h"
#include "enums.h"

class RTC_DS3231 {
   public:
    bool begin(TwoWire* wireInstance = &Wire);
    // void adjust(const DateTime& dt);
    bool lostPower(void);
    uint32_t unixTime();
    Ds3231SqwPinMode readSqwPinMode();
    void writeSqwPinMode(Ds3231SqwPinMode mode);
    // bool setAlarm1(const DateTime& dt, Ds3231Alarm1Mode alarm_mode);
    // bool setAlarm2(const DateTime& dt, Ds3231Alarm2Mode alarm_mode);
    void disableAlarm(uint8_t alarm_num);
    void clearAlarm(uint8_t alarm_num);
    bool alarmFired(uint8_t alarm_num);
    void enable32K(void);
    void disable32K(void);
    bool isEnabled32K(void);
    float getTemperature();  // in Celsius degree

   protected:
    TwoWire* RTCWireBus;  ///< I2C bus connected to the RTC
};

#endif  // _RTCLIB_H_
