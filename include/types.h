#pragma once
#include <string>
#include <vector>
#include <map>
#include <STM32FreeRTOS.h>
#include <SdFat.h>

using StringVector = std::vector<std::string>;
using CharacterVector = std::vector<char*>;
using ProcessMap = std::unordered_map<std::string, TaskHandle_t*>;
using StringPair = std::pair<std::string,std::string>;
typedef int (*parametered_function)(int, char**);
typedef void(function)(void*);
typedef int (*basic_function)();
