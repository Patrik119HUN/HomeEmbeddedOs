#include "boot/kernel_boot.h"
#include "sysvar.h"
#include <DateTime.h>
#include <stm32f0xx_ll_gpio.h>
#include <sysheaders.h>
IFile* rtc = nullptr;
void setup() {
    Debug.timestampOn();
    Debug.setDebugLevel(DBG_VERBOSE);
    kernel_boot();

    VolumeManager* vm = VolumeManager::getInstance();
    IFileSystem* external_drive = vm->getVolume("C");
    if (external_drive != nullptr) {
        IFile* file = external_drive->open("proba2.txt", FILE_WRITE);

        file->println("ez egy alma425");
        file->close();
    }
    rtc = fileSystem.open("/dev/rtc");
    // rtc->ioctl(0,1691254131);
}
void loop() {
    processManager.loop();
    Debug.setTime(rtc->read());
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