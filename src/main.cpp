#include "kernel/kernel_boot.h"
#include "sysvar.h"
#include <sysheaders.h>
#include <stm32f0xx_ll_gpio.h>
void setup() {
    kernel_boot();
    Serial2.begin(9600);
    Serial2.print("s2");
    Serial.print("s1");
    Debug.timestampOn();
    Debug.setDebugLevel(DBG_VERBOSE);
    Debug.setDebugOutputStream(&Serial2);

    VolumeManager* vm = VolumeManager::getInstance();
    IFileSystem* external_drive = vm->getVolume("C");
    if (external_drive != nullptr) {
        IFile* file = external_drive->open("proba2.txt", FILE_WRITE);

        file->println("ez egy alma425");
        file->close();
    }
}

void loop() {
    runner.execute();

    DEBUG_ERROR("alma");
    delay(1000);
    DEBUG_WARNING("GASZ");
    // nm->loop();
}
/*
BUS
    Power
        5V
        3.3V
        12V
        GND
        GND
    Data
        SDA1
        SCL1    I2C

        SDA2
        SCL2    I2C

        MISO
        MOSI    SPI
        SCLK

        1-wire  IDENTIFICATION

        CS1
        CS2
        CS3     Bus Select
        CS4
        CS5
        CS6
*/