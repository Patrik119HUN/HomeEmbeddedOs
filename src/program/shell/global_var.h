#pragma once
#include <STM32FreeRTOS.h>
#include <SdFat.h>
#include <map>
#include <string>
#include <vector>

static std::vector<std::string> paths;
static std::string path_s() {
    std::string path;
    path.append("/");
    for (int i = 0; i < paths.size(); i++) {
        path.append(paths.at(i));
        if (i < paths.size() - 1) {
            path.append("/");
        }
    }
    return path;
}
static FatVolume currentDir;
static std::map<std::string, TaskFunction_t> callbackMap;