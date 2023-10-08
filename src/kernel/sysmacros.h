#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include <types.h>

#include <random>
#include <sstream>
#include <vector>
static int makedev(uint8_t major, uint8_t minor) { return (major * 100) + minor; }
static uint8_t major(int dev) { return dev / 100; }
static uint8_t minor(int dev) { return dev % 100; }

static CharacterVector tokenize(const char* path, const char* delim) {
    CharacterVector folders;
    char *pch, *str = strdup(path);
    pch = strtok(str, delim);
    while (pch != nullptr) {
        folders.push_back(pch);
        pch = strtok(nullptr, delim);
    }
    return folders;
}
static StringVector tokenize_str(const std::string& str, const char delim) {
    StringVector vector;
    std::string token;
    std::istringstream stream(str);

    while (std::getline(stream, token, delim)) {
        vector.push_back(token);
    }
    return vector;
}
static uint8_t read_i2c_register(uint8_t addr, uint8_t reg, TwoWire* wireInstance) {
    wireInstance->beginTransmission(addr);
    wireInstance->write(reg);
    wireInstance->endTransmission();
    wireInstance->requestFrom(addr, (uint8_t)1);
    return wireInstance->read();
}
static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val, TwoWire* wireInstance) {
    wireInstance->beginTransmission(addr);
    wireInstance->write(reg);
    wireInstance->write(val);
    wireInstance->endTransmission();
}
static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }