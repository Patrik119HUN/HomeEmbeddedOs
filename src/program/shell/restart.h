#pragma once
#include <STM32FreeRTOS.h>
#include <Arduino.h>

void restartCallback(void*) {
    Serial.println("System force restart");
    NVIC_SystemReset();
    vTaskDelete(NULL);
}