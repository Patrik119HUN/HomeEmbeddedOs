#pragma once

#include "process_priority.h"

struct processSettings {
    const char* name;
    ProcessPriority priority;
};