#pragma once
/** DS3231 SQW pin mode settings */
enum Ds3231SqwPinMode {
    DS3231_OFF = 0x1C,            /**< Off */
    DS3231_SquareWave1Hz = 0x00,  /**<  1Hz square wave */
    DS3231_SquareWave1kHz = 0x08, /**<  1kHz square wave */
    DS3231_SquareWave4kHz = 0x10, /**<  4kHz square wave */
    DS3231_SquareWave8kHz = 0x18  /**<  8kHz square wave */
};

/** DS3231 Alarm modes for alarm 1 */
enum Ds3231Alarm1Mode {
    DS3231_A1_PerSecond = 0x0F, /**< Alarm once per second */
    DS3231_A1_Second = 0x0E,    /**< Alarm when seconds match */
    DS3231_A1_Minute = 0x0C,    /**< Alarm when minutes and seconds match */
    DS3231_A1_Hour = 0x08,      /**< Alarm when hours, minutes
                                     and seconds match */
    DS3231_A1_Date = 0x00,      /**< Alarm when date (day of month), hours,
                                     minutes and seconds match */
    DS3231_A1_Day = 0x10        /**< Alarm when day (day of week), hours,
                                     minutes and seconds match */
};
/** DS3231 Alarm modes for alarm 2 */
enum Ds3231Alarm2Mode {
    DS3231_A2_PerMinute = 0x7, /**< Alarm once per minute
                                    (whenever seconds are 0) */
    DS3231_A2_Minute = 0x6,    /**< Alarm when minutes match */
    DS3231_A2_Hour = 0x4,      /**< Alarm when hours and minutes match */
    DS3231_A2_Date = 0x0,      /**< Alarm when date (day of month), hours
                                    and minutes match */
    DS3231_A2_Day = 0x8        /**< Alarm when day (day of week), hours
                                    and minutes match */
};