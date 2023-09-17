#pragma once
#include "global_var.h"
#include "kernel/file_system/file_system.h"
#include <Arduino.h>
#include <SdFat.h>
#include <Streaming.h>

void cdCallback(void* arg) {
    std::string& folder = *(std::string*)arg;
    ExFile file;
    ExFile dir;
    if (folder == ".." && paths.size() > 1) {
        paths.pop_back();
    }
    if (!dir.open(path_s().c_str())) {
        syslog(&Serial, Debug_level::ERROR, "Failed to open sd");
    }
    while (file.openNext(&dir, O_RDONLY)) {
        char fname[16];
        file.getName(fname, 16);
        if (folder == fname) {
            if (file.isDir()) {
                paths.push_back(folder);
            }
        }
        file.close();
    }
    vTaskDelete(NULL);
}