#ifndef sysvar_h
#define sysvar_h

#include "sysheaders.h"
#include <TaskScheduler.h>
#include <WiFiUdp.h>
#define BAUD_RATE 9600
#define SD_PIN PB6
#define W5500_PIN PA9

//Scheduler runner;
rtc rtcdev;
//void proba_callback() {
//    char* argv[3] = {"alma", "banan", "kukorica "};
//    int a = ntp_deamon(3, argv);
//    DEBUG_INFO("Function returned with %d", a);
//};
//Task t1(100 * TASK_MILLISECOND, TASK_FOREVER, &cli_wrapper, &runner);
//Task NTP_deamon(TASK_MINUTE, TASK_FOREVER, &ntp_wrapper, &runner);
//Task proba(TASK_MINUTE, TASK_ONCE, &proba_callback, &runner);
full fulldev;
zero zerodev;
null nulldev;
terminal term;

#endif // sysvar