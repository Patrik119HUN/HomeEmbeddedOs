#ifndef rtc_const_h
#define rtc_const_h

#define DS3231_ADDRESS 0x68   ///< I2C address for DS3231
#define DS3231_TIME 0x00      ///< Time register
#define DS3231_ALARM1 0x07    ///< Alarm 1 register
#define DS3231_ALARM2 0x0B    ///< Alarm 2 register
#define DS3231_CONTROL 0x0E   ///< Control register
#define DS3231_STATUSREG 0x0F ///< Status register
#define DS3231_TEMPERATUREREG                                                                      \
    0x11 ///< Temperature register (high byte - low byte is at 0x12), 10-bit
         ///< temperature value

/** Constants */
#define SECONDS_PER_DAY 86400L ///< 60 * 60 * 24
#define SECONDS_FROM_1970_TO_2000                                                                  \
    946684800 ///< Unixtime for 2000-01-01 00:00:00, useful for initialization

#endif // const