#ifndef SYSMACROS_H
#define SYSMACROS_H

#include <Arduino.h>
#include <string.h>

#include <vector>

static dev_t makedev(uint8_t major, uint8_t minor) { return (major * 100) + minor; }
static uint8_t major(dev_t dev) { return dev / 100; }
static uint8_t minor(dev_t dev) { return dev % 100; }

static std::vector<char*> tokenize(const char* path, const char* delim) {
    std::vector<char*> folders;
    char *pch, *str = strdup(path);
    pch = strtok(str, delim);
    while (pch != nullptr) {
        folders.push_back(pch);
        pch = strtok(nullptr, delim);
    }
    return folders;
}


#endif // SYSMACROS_H
