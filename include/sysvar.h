#ifndef sysvar_h
#define sysvar_h

#include "sysheaders.h"

#include <TaskScheduler.h>
#define BAUD_RATE 9600
#define SD_PIN PB6
#define W5500_PIN PA9

Scheduler runner;
console cli;
void cli_wrapper() { cli.loop(); }
Task t1(100 * TASK_MILLISECOND, TASK_FOREVER, &cli_wrapper, &runner);
full fulldev;
zero zerodev;
null nulldev;
terminal term;
rtc rtcdev;

#endif // sysvar