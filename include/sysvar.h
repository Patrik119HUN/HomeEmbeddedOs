#pragma once

#include "sysheaders.h"
#include <TaskScheduler.h>
#include <WiFiUdp.h>
#define BAUD_RATE 9600
#define SD_PIN PB6
#define W5500_PIN PA9

rtc rtcdev;
full fulldev;
zero zerodev;
null nulldev;
terminal term;