#pragma once

#include "process_priority.h"
#include <string>
struct processSettings {
    std::string name;
    ProcessPriority priority;
};