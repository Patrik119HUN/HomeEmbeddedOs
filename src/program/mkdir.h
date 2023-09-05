#pragma once
#include <Arduino.h>
#include <Streaming.h>

#include "kernel/file_system/file_system.h"

int mkdirCallback(int argc, char** argv) { fileSystem.mkdir(argv[1]); return 0;}