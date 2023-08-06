#ifndef time_h
#define time_h

#include "../sysheaders.h"
inline uint32_t getEpoch() {
    FileSystem* fs = FileSystem::getInstance();
    IFile* rtc = fs->open("/dev/rtc");
    if (rtc == nullptr) return 0;
    if (rtc->available() == 0) return 1;
    return rtc->read();
}
uint32_t getYears() { return getEpoch() / 86400L / 365 + 1970; }
uint32_t getMonths() { return getEpoch() / 2629743L % 12; };
uint8_t getDays() { return (((getEpoch() / 86400L) + 4) % 7); };
uint8_t getHours() { return (getEpoch() % 86400L) / 3600; };
uint8_t getMinutes() { return (getEpoch() % 3600) / 60; };
uint8_t getSeconds() { return getEpoch() % 60; }
#endif // time