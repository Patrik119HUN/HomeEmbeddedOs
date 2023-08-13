#ifndef ntp_deamon_h
#define ntp_deamon_h

#include "kernel/file_system/file_system.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <Streaming.h>
#include <WiFiUdp.h>
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp);

int ntp_deamon(int argc, char** argv) {
    timeClient.begin();
    IFile* rtc = FileSystem::getInstance()->open("/dev/rtc");
    if (rtc == nullptr) return 1;
    while (true) {
        timeClient.update();
        rtc->ioctl(0, timeClient.getEpochTime());
    }
    return 0;
}
#endif // ntp_deamon