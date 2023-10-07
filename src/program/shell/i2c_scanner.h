#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <Wire.h>
#include <sys/log.h>

#include <sstream>
void i2c_scanner(void*) {
    Wire.begin();
    byte error;
    int nDevices = 0;
    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            std::stringstream buffer;
            buffer << "I2C device found at address 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)address << " !";
            INFO(buffer.str().c_str());
            nDevices++;
        }
    }
    if (nDevices == 0) INFO("No I2C devices found\n");
    Wire.end();
    vTaskDelete(NULL);
}