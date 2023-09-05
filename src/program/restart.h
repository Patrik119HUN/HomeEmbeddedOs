#pragma once

#include <Arduino.h>

int restartCallback(int argc, char** argv) {
    Serial.println("System force restart");
    NVIC_SystemReset();
    return 0;
}