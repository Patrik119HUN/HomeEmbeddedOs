#include "RTClib.h"

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#elif defined(ARDUINO_ARCH_SAMD)
// nothing special needed
#elif defined(ARDUINO_SAM_DUE)
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char*)(addr))
#endif

#if (ARDUINO >= 100)
#define _I2C_WRITE write ///< Modern I2C write
#define _I2C_READ read   ///< Modern I2C read
#else
#include <WProgram.h>
#define _I2C_WRITE send   ///< Legacy I2C write
#define _I2C_READ receive ///< legacy I2C read
#endif

/**************************************************************************/
/*!
    @brief  Read a byte from an I2C register
    @param addr I2C address
    @param reg Register address
    @return Register value
*/
/**************************************************************************/
static uint8_t read_i2c_register(uint8_t addr, uint8_t reg, TwoWire* wireInstance) {
    wireInstance->beginTransmission(addr);
    wireInstance->_I2C_WRITE((byte)reg);
    wireInstance->endTransmission();

    wireInstance->requestFrom(addr, (byte)1);
    return wireInstance->_I2C_READ();
}

/**************************************************************************/
/*!
    @brief  Write a byte to an I2C register
    @param addr I2C address
    @param reg Register address
    @param val Value to write
*/
/**************************************************************************/
static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val, TwoWire* wireInstance) {
    wireInstance->beginTransmission(addr);
    wireInstance->_I2C_WRITE((byte)reg);
    wireInstance->_I2C_WRITE((byte)val);
    wireInstance->endTransmission();
}

/**************************************************************************/
// utility code, some of this could be exposed in the DateTime API if needed
/**************************************************************************/

/**
  Number of days in each month, from January to November. December is not
  needed. Omitting it avoids an incompatibility with Paul Stoffregen's Time
  library. C.f. https://github.com/adafruit/RTClib/issues/114
*/
const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

/**************************************************************************/
/*!
    @brief  Given a date, return number of days since 2000/01/01,
            valid for 2000--2099
    @param y Year
    @param m Month
    @param d Day
    @return Number of days
*/
/**************************************************************************/
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000U) y -= 2000U;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0) ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

/**************************************************************************/
/*!
    @brief  Given a number of days, hours, minutes, and seconds, return the
   total seconds
    @param days Days
    @param h Hours
    @param m Minutes
    @param s Seconds
    @return Number of seconds total
*/
/**************************************************************************/
static uint32_t time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24UL + h) * 60 + m) * 60 + s;
}

/**************************************************************************/
/*!
    @brief  Convert a binary coded decimal value to binary. RTC stores time/date
   values as BCD.
    @param val BCD value
    @return Binary value
*/
/**************************************************************************/
static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }

/**************************************************************************/
/*!
    @brief  Convert a binary value to BCD format for the RTC registers
    @param val Binary value
    @return BCD value
*/
/**************************************************************************/
static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }

/**************************************************************************/
/*!
    @brief  Convert the day of the week to a representation suitable for
            storing in the DS3231: from 1 (Monday) to 7 (Sunday).
    @param  d Day of the week as represented by the library:
            from 0 (Sunday) to 6 (Saturday).
*/
/**************************************************************************/
static uint8_t dowToDS3231(uint8_t d) { return d == 0 ? 7 : d; }

/**************************************************************************/
/*!
    @brief  Start I2C for the DS3231 and test succesful connection
    @param  wireInstance pointer to the I2C bus
    @return True if Wire can find DS3231 or false otherwise.
*/
/**************************************************************************/

bool RTC_DS3231::begin(TwoWire* wireInstance) {
    RTCWireBus = wireInstance;
    wireInstance->setSCL(PB8);
    wireInstance->setSDA(PB9);
    RTCWireBus->begin();
    RTCWireBus->beginTransmission(DS3231_ADDRESS);
    if (RTCWireBus->endTransmission() == 0) return true;
    return false;
}

/**************************************************************************/
/*!
    @brief  Check the status register Oscillator Stop Flag to see if the DS3231
   stopped due to power loss
    @return True if the bit is set (oscillator stopped) or false if it is
   running
*/
/**************************************************************************/
bool RTC_DS3231::lostPower(void) {
    return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus) >> 7);
}

/**************************************************************************/
/*!
    @brief  Set the date and flip the Oscillator Stop Flag
    @param dt DateTime object containing the date/time to set
*/
/**************************************************************************/
void RTC_DS3231::adjust(const DateTime& dt) {
    RTCWireBus->beginTransmission(DS3231_ADDRESS);
    RTCWireBus->_I2C_WRITE((byte)DS3231_TIME); // start at location 0
    RTCWireBus->_I2C_WRITE(bin2bcd(dt.second()));
    RTCWireBus->_I2C_WRITE(bin2bcd(dt.minute()));
    RTCWireBus->_I2C_WRITE(bin2bcd(dt.hour()));
    // The RTC must know the day of the week for the weekly alarms to work.
    RTCWireBus->_I2C_WRITE(bin2bcd(dowToDS3231(dt.dayOfTheWeek())));
    RTCWireBus->_I2C_WRITE(bin2bcd(dt.day()));
    RTCWireBus->_I2C_WRITE(bin2bcd(dt.month()));
    RTCWireBus->_I2C_WRITE(bin2bcd(dt.year() - 2000U));
    RTCWireBus->endTransmission();

    uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
    statreg &= ~0x80; // flip OSF bit
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg, RTCWireBus);
}

/**************************************************************************/
/*!
    @brief  Get the current date/time
    @return DateTime object with the current date/time
*/
/**************************************************************************/
uint32_t RTC_DS3231::unixTime() {
    RTCWireBus->beginTransmission(DS3231_ADDRESS);
    RTCWireBus->_I2C_WRITE((byte)0);
    RTCWireBus->endTransmission();

    RTCWireBus->requestFrom(DS3231_ADDRESS, 7);
    uint8_t ss = bcd2bin(RTCWireBus->_I2C_READ() & 0x7F);
    uint8_t mm = bcd2bin(RTCWireBus->_I2C_READ());
    uint8_t hh = bcd2bin(RTCWireBus->_I2C_READ());
    RTCWireBus->_I2C_READ();
    uint8_t d = bcd2bin(RTCWireBus->_I2C_READ());
    uint8_t m = bcd2bin(RTCWireBus->_I2C_READ());
    uint16_t y = bcd2bin(RTCWireBus->_I2C_READ()) + 2000U;

    if (y >= 2000U) y -= 2000U;
    uint16_t yOff = y;
    uint32_t t;
    uint16_t days = date2days(yOff, m, d);
    t = time2ulong(days, hh, mm, ss);
    t += SECONDS_FROM_1970_TO_2000; // seconds from 1970 to 2000

    return t;
}

/**************************************************************************/
/*!
    @brief  Read the SQW pin mode
    @return Pin mode, see Ds3231SqwPinMode enum
*/
/**************************************************************************/
Ds3231SqwPinMode RTC_DS3231::readSqwPinMode() {
    int mode;

    RTCWireBus->beginTransmission(DS3231_ADDRESS);
    RTCWireBus->_I2C_WRITE(DS3231_CONTROL);
    RTCWireBus->endTransmission();

    RTCWireBus->requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)1);
    mode = RTCWireBus->_I2C_READ();

    mode &= 0x1C;
    if (mode & 0x04) mode = DS3231_OFF;
    return static_cast<Ds3231SqwPinMode>(mode);
}

/**************************************************************************/
/*!
    @brief  Set the SQW pin mode
    @param mode Desired mode, see Ds3231SqwPinMode enum
*/
/**************************************************************************/
void RTC_DS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
    uint8_t ctrl;
    ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);

    ctrl &= ~0x04; // turn off INTCON
    ctrl &= ~0x18; // set freq bits to 0

    ctrl |= mode;
    write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl, RTCWireBus);

    // Serial.println( read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL,
    // RTCWireBus), HEX);
}

/**************************************************************************/
/*!
    @brief  Get the current temperature from the DS3231's temperature sensor
    @return Current temperature (float)
*/
/**************************************************************************/
float RTC_DS3231::getTemperature() {
    uint8_t lsb;
    int8_t msb;
    RTCWireBus->beginTransmission(DS3231_ADDRESS);
    RTCWireBus->_I2C_WRITE(DS3231_TEMPERATUREREG);
    RTCWireBus->endTransmission();

    RTCWireBus->requestFrom(DS3231_ADDRESS, 2);
    msb = RTCWireBus->_I2C_READ();
    lsb = RTCWireBus->_I2C_READ();

    //  Serial.print("msb=");
    //  Serial.print(msb,HEX);
    //  Serial.print(", lsb=");
    //  Serial.println(lsb,HEX);

    return (float)msb + (lsb >> 6) * 0.25f;
}

/**************************************************************************/
/*!
    @brief  Set alarm 1 for DS3231
        @param 	dt DateTime object
        @param 	alarm_mode Desired mode, see Ds3231Alarm1Mode enum
    @return False if control register is not set, otherwise true
*/
/**************************************************************************/
// bool RTC_DS3231::setAlarm1(const DateTime& dt, Ds3231Alarm1Mode alarm_mode) {
//     uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);
//     if (!(ctrl & 0x04)) {
//         return false;
//     }

//     uint8_t A1M1 = (alarm_mode & 0x01) << 7;   // Seconds bit 7.
//     uint8_t A1M2 = (alarm_mode & 0x02) << 6;   // Minutes bit 7.
//     uint8_t A1M3 = (alarm_mode & 0x04) << 5;   // Hour bit 7.
//     uint8_t A1M4 = (alarm_mode & 0x08) << 4;   // Day/Date bit 7.
//     uint8_t DY_DT = (alarm_mode & 0x10) << 2;  // Day/Date bit 6. Date when 0, day of week
//     when 1.

//     RTCWireBus->beginTransmission(DS3231_ADDRESS);
//     RTCWireBus->_I2C_WRITE(DS3231_ALARM1);
//     RTCWireBus->_I2C_WRITE(bin2bcd(dt.second()) | A1M1);
//     RTCWireBus->_I2C_WRITE(bin2bcd(dt.minute()) | A1M2);
//     RTCWireBus->_I2C_WRITE(bin2bcd(dt.hour()) | A1M3);
//     if (DY_DT) {
//         RTCWireBus->_I2C_WRITE(bin2bcd(dowToDS3231(dt.dayOfTheWeek())) | A1M4 | DY_DT);
//     } else {
//         RTCWireBus->_I2C_WRITE(bin2bcd(dt.day()) | A1M4 | DY_DT);
//     }
//     RTCWireBus->endTransmission();

//     ctrl |= 0x01;  // AI1E
//     write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl, RTCWireBus);
//     return true;
// }

/**************************************************************************/
/*!
    @brief  Set alarm 2 for DS3231
        @param 	dt DateTime object
        @param 	alarm_mode Desired mode, see Ds3231Alarm2Mode enum
    @return False if control register is not set, otherwise true
*/
/**************************************************************************/
// bool RTC_DS3231::setAlarm2(const DateTime& dt, Ds3231Alarm2Mode alarm_mode) {
//     uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);
//     if (!(ctrl & 0x04)) {
//         return false;
//     }

//     uint8_t A2M2 = (alarm_mode & 0x01) << 7;  // Minutes bit 7.
//     uint8_t A2M3 = (alarm_mode & 0x02) << 6;  // Hour bit 7.
//     uint8_t A2M4 = (alarm_mode & 0x04) << 5;  // Day/Date bit 7.
//     uint8_t DY_DT = (alarm_mode & 0x8) << 3;  // Day/Date bit 6. Date when 0, day of week when 1.

//     RTCWireBus->beginTransmission(DS3231_ADDRESS);
//     RTCWireBus->_I2C_WRITE(DS3231_ALARM2);
//     RTCWireBus->_I2C_WRITE(bin2bcd(dt.minute()) | A2M2);
//     RTCWireBus->_I2C_WRITE(bin2bcd(dt.hour()) | A2M3);
//     if (DY_DT) {
//         RTCWireBus->_I2C_WRITE(bin2bcd(dowToDS3231(dt.dayOfTheWeek())) | A2M4 | DY_DT);
//     } else {
//         RTCWireBus->_I2C_WRITE(bin2bcd(dt.day()) | A2M4 | DY_DT);
//     }
//     RTCWireBus->endTransmission();

//     ctrl |= 0x02;  // AI2E
//     write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl, RTCWireBus);
//     return true;
// }

/**************************************************************************/
/*!
    @brief  Disable alarm
        @param 	alarm_num Alarm number to disable
*/
/**************************************************************************/
void RTC_DS3231::disableAlarm(uint8_t alarm_num) {
    uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);
    ctrl &= ~(1 << (alarm_num - 1));
    write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl, RTCWireBus);
}

/**************************************************************************/
/*!
    @brief  Clear status of alarm
        @param 	alarm_num Alarm number to clear
*/
/**************************************************************************/
void RTC_DS3231::clearAlarm(uint8_t alarm_num) {
    uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
    status &= ~(0x1 << (alarm_num - 1));
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
}

/**************************************************************************/
/*!
    @brief  Get status of alarm
        @param 	alarm_num Alarm number to check status of
        @return True if alarm has been fired otherwise false
*/
/**************************************************************************/
bool RTC_DS3231::alarmFired(uint8_t alarm_num) {
    uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
    return (status >> (alarm_num - 1)) & 0x1;
}

/**************************************************************************/
/*!
    @brief  Enable 32KHz Output
    @details The 32kHz output is enabled by default. It requires an external
    pull-up resistor to function correctly
*/
/**************************************************************************/
void RTC_DS3231::enable32K(void) {
    uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
    status |= (0x1 << 0x03);
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
    // Serial.println(read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG,
    // RTCWireBus), BIN);
}

/**************************************************************************/
/*!
    @brief  Disable 32KHz Output
*/
/**************************************************************************/
void RTC_DS3231::disable32K(void) {
    uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
    status &= ~(0x1 << 0x03);
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
    // Serial.println(read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG,
    // RTCWireBus), BIN);
}

/**************************************************************************/
/*!
    @brief  Get status of 32KHz Output
    @return True if enabled otherwise false
*/
/**************************************************************************/
bool RTC_DS3231::isEnabled32K(void) {
    uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, RTCWireBus);
    return (status >> 0x03) & 0x1;
}
