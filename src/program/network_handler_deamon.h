#pragma once
#include "../kernel/network_manager/network_manager.h"
#include <STM32FreeRTOS.h>
void network_handler_deamon(void* argv) {
    UNUSED(argv);
    while (true) {
        taskYIELD();
    }
}