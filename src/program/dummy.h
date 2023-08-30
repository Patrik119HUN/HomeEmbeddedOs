#pragma once
#include <Arduino.h>
#include <Streaming.h>

int dummy(int argc, char** argv) {
    Serial << "You have entered " << argc << " arguments:"
           << "\n";

    for (int i = 0; i < argc; ++i)
        Serial << argv[i] << "\n";

    return 0;
}