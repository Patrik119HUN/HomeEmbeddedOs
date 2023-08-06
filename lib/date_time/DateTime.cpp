#include "DateTime.h"

const uint8_t daysInMonth[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000U) y -= 2000U;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += daysInMonth[i - 1];
    if (m > 2 && y % 4 == 0) ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

static uint32_t time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24UL + h) * 60 + m) * 60 + s;
}

DateTime::DateTime(uint32_t t) { this->setEpoch(t); }

void DateTime::setEpoch(uint32_t t) {
    t -= SECONDS_FROM_1970_TO_2000; // bring to 2000 timestamp from 1970

    ss = t % 60;
    t /= 60;
    mm = t % 60;
    t /= 60;
    hh = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (yOff = 0;; ++yOff) {
        leap = yOff % 4 == 0;
        if (days < 365U + leap) break;
        days -= 365 + leap;
    }
    for (m = 1; m < 12; ++m) {
        uint8_t daysPerMonth = daysInMonth[m - 1];
        if (leap && m == 2) ++daysPerMonth;
        if (days < daysPerMonth) break;
        days -= daysPerMonth;
    }
    d = days + 1;
}
DateTime::DateTime(
    uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec
) {
    if (year >= 2000U) year -= 2000U;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

DateTime::DateTime(const DateTime& copy)
    : yOff(copy.yOff), m(copy.m), d(copy.d), hh(copy.hh), mm(copy.mm), ss(copy.ss) {}

uint8_t DateTime::dayOfTheWeek() const {
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t DateTime::unixtime(void) const {
    uint32_t t;
    uint16_t days = date2days(yOff, m, d);
    t = time2ulong(days, hh, mm, ss);
    t += SECONDS_FROM_1970_TO_2000; // seconds from 1970 to 2000

    return t;
}

uint32_t DateTime::secondstime(void) const {
    uint32_t t;
    uint16_t days = date2days(yOff, m, d);
    t = time2ulong(days, hh, mm, ss);
    return t;
}

DateTime DateTime::operator+(const TimeSpan& span) const {
    return DateTime(unixtime() + span.totalseconds());
}

DateTime DateTime::operator-(const TimeSpan& span) const {
    return DateTime(unixtime() - span.totalseconds());
}

TimeSpan DateTime::operator-(const DateTime& right) const {
    return TimeSpan(unixtime() - right.unixtime());
}

bool DateTime::operator<(const DateTime& right) const {
    return (
        yOff + 2000U < right.year() ||
        (yOff + 2000U == right.year() &&
         (m < right.month() ||
          (m == right.month() &&
           (d < right.day() ||
            (d == right.day() &&
             (hh < right.hour() ||
              (hh == right.hour() &&
               (mm < right.minute() || (mm == right.minute() && ss < right.second())))))))))
    );
}

bool DateTime::operator==(const DateTime& right) const {
    return (
        right.year() == yOff + 2000U && right.month() == m && right.day() == d &&
        right.hour() == hh && right.minute() == mm && right.second() == ss
    );
}

String DateTime::timestamp(timestampOpt opt) const {
    char buffer[25]; // large enough for any DateTime, including invalid ones

    // Generate timestamp according to opt
    switch (opt) {
    case TIMESTAMP_TIME:
        // Only time
        sprintf(buffer, "%02d:%02d:%02d", hh, mm, ss);
        break;
    case TIMESTAMP_DATE:
        // Only date
        sprintf(buffer, "%u-%02d-%02d", 2000U + yOff, m, d);
        break;
    default:
        // Full
        sprintf(buffer, "%u-%02d-%02dT%02d:%02d:%02d", 2000U + yOff, m, d, hh, mm, ss);
    }
    return String(buffer);
}
