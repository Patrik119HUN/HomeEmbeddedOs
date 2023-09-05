#pragma once
#include <Arduino.h>
#include <Streaming.h>

int echoCallback(int argc, char** argv) {
    Serial << argv[1] << "\n";
    return 0;
}