#pragma once

#include <Arduino.h>

constexpr uint8_t LCD_COLS = 20;
constexpr uint8_t LCD_ROWS = 4;

constexpr uint8_t rs = PC8;
constexpr uint8_t rw = PC6;
constexpr uint8_t en = PC5;
constexpr uint8_t d0 = PA12;
constexpr uint8_t d1 = PA11;
constexpr uint8_t d2 = PB12;
constexpr uint8_t d3 = PB11;
constexpr uint8_t d4 = PB2;
constexpr uint8_t d5 = PB1;
constexpr uint8_t d6 = PB15;
constexpr uint8_t d7 = PB14;