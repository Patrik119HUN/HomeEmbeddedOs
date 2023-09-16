#ifndef DateTime_h
#define DateTime_h

#include <Arduino.h>
#include "const.h"
#include "time_span.h"

class DateTime {
  public:
    DateTime(uint32_t t = SECONDS_FROM_1970_TO_2000);
    DateTime(
        uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0,
        uint8_t sec = 0
    );
    
    DateTime(const DateTime& copy);

    uint16_t year() const { return 2000U + yOff; }
    uint8_t month() const { return m; }
    uint8_t day() const { return d; }
    uint8_t hour() const { return hh; }

    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }

    uint8_t dayOfTheWeek() const;
    uint32_t secondstime() const;
    uint32_t unixtime(void) const;
    void setEpoch(uint32_t t);
    enum timestampOpt {
        TIMESTAMP_FULL, //!< `YYYY-MM-DDThh:mm:ss`
        TIMESTAMP_TIME, //!< `hh:mm:ss`
        TIMESTAMP_DATE  //!< `YYYY-MM-DD`
    };
    String timestamp(timestampOpt opt = TIMESTAMP_FULL) const;

    DateTime operator+(const TimeSpan& span) const;
    DateTime operator-(const TimeSpan& span) const;
    TimeSpan operator-(const DateTime& right) const;

    bool operator<(const DateTime& right) const;

    bool operator>(const DateTime& right) const { return right < *this; }
    bool operator<=(const DateTime& right) const { return !(*this > right); }

    bool operator>=(const DateTime& right) const { return !(*this < right); }
    bool operator==(const DateTime& right) const;

    bool operator!=(const DateTime& right) const { return !(*this == right); }

  protected:
    uint8_t yOff; ///< Year offset from 2000
    uint8_t m;    ///< Month 1-12
    uint8_t d;    ///< Day 1-31
    uint8_t hh;   ///< Hours 0-23
    uint8_t mm;   ///< Minutes 0-59
    uint8_t ss;   ///< Seconds 0-59
};

#endif // DateTime