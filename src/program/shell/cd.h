#pragma once
#include "global_var.h"
#include "kernel/file_system/file_system.h"
#include <Arduino.h>
#include <SdFat.h>
#include <Streaming.h>

void cdCallback(void* arg) {
    std::string& folder = *(std::string*)arg;
    if (sd.chdir(folder.c_str())) {
        paths.push_back(folder);
    }
    vTaskDelete(NULL);
}