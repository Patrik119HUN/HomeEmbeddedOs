#pragma once
#include <Arduino.h>
#include <DateTime.h>
#include <Wire.h>
#include <real_time_clock_interface.h>

#include "../../kernel/sysmacros.h"
#include "const.h"
#include "enums.h"

const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000U) y -= 2000U;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i) days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0) ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}
static uint32_t time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s) { return ((days * 24UL + h) * 60 + m) * 60 + s; }
static uint8_t dowToDS3231(uint8_t d) { return d == 0 ? 7 : d; }

class RTC_DS3231 : public IRTC {
   public:
    bool begin(TwoWire* wireInstance = &Wire) override {
        RTCWireBus = wireInstance;
        wireInstance->setSCL(PB8);
        wireInstance->setSDA(PB9);
        RTCWireBus->begin();
        RTCWireBus->beginTransmission(DS3231_ADDRESS);
        if (RTCWireBus->endTransmission() == 0) return true;
        return false;
    }
    void set_time(const DateTime& dt) override {
        RTCWireBus->beginTransmission(DS3231_ADDRESS);
        RTCWireBus->write((byte)DS3231_TIME);  // start at location 0
        RTCWireBus->write(bin2bcd(dt.second()));
        RTCWireBus->write(bin2bcd(dt.minute()));
        RTCWireBus->write(bin2bcd(dt.hour()));
        // The RTC must know the day of the week for the weekly alarms to work.
        RTCWireBus->write(bin2bcd(dowToDS3231(dt.dayOfTheWeek())));
        RTCWireBus->write(bin2bcd(dt.day()));
        RTCWireBus->write(bin2bcd(dt.month()));
        RTCWireBus->write(bin2bcd(dt.year() - 2000U));
        RTCWireBus->endTransmission();

        uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
        statreg &= ~0x80;  // flip OSF bit
        write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg, RTCWireBus);
    }
    bool lost_power(void) override { return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus) >> 7); }
    uint32_t get_time() override {
        RTCWireBus->beginTransmission(DS3231_ADDRESS);
        RTCWireBus->write((byte)0);
        RTCWireBus->endTransmission();

        RTCWireBus->requestFrom(DS3231_ADDRESS, 7);
        uint8_t ss = bcd2bin(RTCWireBus->read() & 0x7F);
        uint8_t mm = bcd2bin(RTCWireBus->read());
        uint8_t hh = bcd2bin(RTCWireBus->read());
        RTCWireBus->read();
        uint8_t d = bcd2bin(RTCWireBus->read());
        uint8_t m = bcd2bin(RTCWireBus->read());
        uint16_t y = bcd2bin(RTCWireBus->read()) + 2000U;

        if (y >= 2000U) y -= 2000U;
        uint16_t yOff = y;
        uint32_t t;
        uint16_t days = date2days(yOff, m, d);
        t = time2ulong(days, hh, mm, ss);
        t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

        return t;
    }

    void disable_alarm(uint8_t alarm_num) override {
        uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);
        ctrl &= ~(1 << (alarm_num - 1));
        write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl, RTCWireBus);
    }
    void clear_alarm(uint8_t alarm_num) override {
        uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
        status &= ~(0x1 << (alarm_num - 1));
        write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
    }
    bool alarm_fired(uint8_t alarm_num) override {
        uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
        return (status >> (alarm_num - 1)) & 0x1;
    }

    Ds3231SqwPinMode readSqwPinMode() {
        int mode;

        RTCWireBus->beginTransmission(DS3231_ADDRESS);
        RTCWireBus->write(DS3231_CONTROL);
        RTCWireBus->endTransmission();

        RTCWireBus->requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)1);
        mode = RTCWireBus->read();

        mode &= 0x1C;
        if (mode & 0x04) mode = DS3231_OFF;
        return static_cast<Ds3231SqwPinMode>(mode);
    }
    void writeSqwPinMode(Ds3231SqwPinMode mode) {
        uint8_t ctrl;
        ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);

        ctrl &= ~0x04;  // turn off INTCON
        ctrl &= ~0x18;  // set freq bits to 0

        ctrl |= mode;
        write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl, RTCWireBus);
    }
    // bool setAlarm1(const DateTime& dt, Ds3231Alarm1Mode alarm_mode);
    // bool setAlarm2(const DateTime& dt, Ds3231Alarm2Mode alarm_mode);
    void enable32K(void) {
        uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
        status |= (0x1 << 0x03);
        write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
    }
    void disable32K(void) {
        uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
        status &= ~(0x1 << 0x03);
        write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
    }
    bool isEnabled32K(void) {
        uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
        return (status >> 0x03) & 0x1;
    }

    float getTemperature() {
        uint8_t lsb;
        int8_t msb;
        RTCWireBus->beginTransmission(DS3231_ADDRESS);
        RTCWireBus->write(DS3231_TEMPERATUREREG);
        RTCWireBus->endTransmission();

        RTCWireBus->requestFrom(DS3231_ADDRESS, 2);
        msb = RTCWireBus->read();
        lsb = RTCWireBus->read();
        return (float)msb + (lsb >> 6) * 0.25f;
    }

   protected:
    TwoWire* RTCWireBus;  ///< I2C bus connected to the RTC
};