#ifndef time_span_h
#define time_span_h

#include <Arduino.h>

class TimeSpan {
  public:
    TimeSpan(uint32_t seconds) : _seconds(seconds) {}

    TimeSpan(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
        : _seconds(
              (uint32_t)days * 86400L + (uint32_t)hours * 3600 + (uint32_t)minutes * 60 + seconds
          ) {}

    TimeSpan(const TimeSpan& copy) : _seconds(copy._seconds) {}

    uint16_t days() const { return _seconds / 86400L; }
    uint8_t hours() const { return _seconds / 3600 % 24; }
    uint8_t minutes() const { return _seconds / 60 % 60; }
    uint8_t seconds() const { return _seconds % 60; }
    uint32_t totalseconds() const { return _seconds; }

    TimeSpan operator+(const TimeSpan& right) const { return TimeSpan(_seconds + right._seconds); }
    TimeSpan operator-(const TimeSpan& right) const { return TimeSpan(_seconds - right._seconds); }

  protected:
    uint32_t _seconds;
};

#endif // time_span