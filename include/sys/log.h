#pragma once
#include <array>
#include <map>
#include <stdarg.h>

enum class Debug_level { EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG };

std::array<const char*, 12> months = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
std::map<Debug_level, const char*> errorCode{
    {Debug_level::EMERGENCY, "Emergency"}, {Debug_level::ALERT, "ALERT"},
    {Debug_level::CRITICAL, "CRITICAL"},   {Debug_level::ERROR, "ERROR"},
    {Debug_level::WARNING, "WARNING"},     {Debug_level::NOTICE, "NOTICE"},
    {Debug_level::INFO, "INFO"},           {Debug_level::DEBUG, "DEBUG"}};

void syslog(Stream* where, Debug_level level, const char* format, ...) {
    char buffer[128];
    DateTime time;

    time.setEpoch(deviceManager.open("rtc")->read());

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 128, format, args);
    va_end(args);

    where->printf(
        "%s %02d %02d:%02d:%02d ", months.at(time.month() - 1), time.day(), time.hour(),
        time.minute(), time.second()
    );
    where->printf("[%s]", errorCode[level]);
    where->print(buffer);
    where->print("\n");
}