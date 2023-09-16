#include "../kernel/kernel.h"
#include <Arduino.h>
#include <Streaming.h>
#include <uuid.h>
#include <sys/log.h>

void randomGen(void* param) {
    UNUSED(param);
    UUID id;
    while (true) {
        syslog(&Serial, Debug_level::EMERGENCY, "alma");
        syslog(&Serial, Debug_level::ERROR, "omg cucc");
        taskYIELD();
    }
}
/*
    #include <syslog.h>

    void syslog(int priority,const char* message, arguments...);
    LOG_EMERG- emergency
    LOG_ALERT -high priority problem, -> database corruption
    LOG_CRIT -> critical problem -> hardware fail
    LOG_ERR -> error
    LOG_Waring -> Warning
    LOG_NOTICE -> special condition
    LOG_INFO -> inform
    LOG_DEBUG -> debug

*/