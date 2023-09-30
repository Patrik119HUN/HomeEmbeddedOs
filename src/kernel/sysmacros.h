#pragma once
#include <Arduino.h>
#include <string.h>

#include <random>
#include <sstream>
#include <vector>
static int makedev(uint8_t major, uint8_t minor) { return (major * 100) + minor; }
static uint8_t major(int dev) { return dev / 100; }
static uint8_t minor(int dev) { return dev % 100; }

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
static std::vector<std::string> tokenize_str(const std::string& str, const char delim) {
    std::vector<std::string> vector;
    std::string token;
    std::istringstream stream(str);

    while (std::getline(stream, token, delim)) {
        vector.push_back(token);
    }
    return vector;
}