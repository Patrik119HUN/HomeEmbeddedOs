#include <System/Driver/full/full.h>
#include <System/Driver/null/null.h>
#include <System/Driver/zero/zero.h>
#include <System/volume_manager/volume_manager.h>
#include <TaskScheduler.h>
#include <sysheaders.h>

#include <vector>

#include "System/Driver/SD/SD.h"

Scheduler runner;
FileSystem* fs = FileSystem::getInstance();
console cli;
DeviceManager* dv = DeviceManager::GetInstance();
VolumeManager* vm = VolumeManager::GetInstance();

void cli_wrapper() { cli.loop(); }
Task t1(100 * TASK_MILLISECOND, TASK_FOREVER, &cli_wrapper, &runner);
// full fulldev;
zero zerodev;
null nulldev;
terminal term;

Screen* scr;
int main(void) {
    init();
    initVariant();
    randomSeed(analogRead(A0));
    fs->mkdir("/dev");
    // scr = new Screen;
    // fs->mknod("/dev/full", dv->addDev(DeviceManager::devTypes::sys, &fulldev));
    // fs->mknod("/dev/zero", dv->addDev(DeviceManager::devTypes::sys, &nulldev));
    // fs->mknod("/dev/null", dv->addDev(DeviceManager::devTypes::sys, &zerodev));
    // fs->mknod("/dev/scr", dv->addDev(DeviceManager::devTypes::screen, scr));
    // fs->mknod("/dev/tty", dv->addDev(DeviceManager::devTypes::screen, &term));
    // fs->mkdir("/user");

    Serial.begin(9600);
    if (!SD.begin(PB6)) {
        Serial.println("initialization failed!");
        while (1)
            ;
    }
    vm->Mount("C", FSType::kFAT32, &SD);
    IFileSystem* external_drive = vm->GetVolume("C");
    if (external_drive != nullptr) {
        IFile* file = external_drive->open("proba2.txt", FILE_WRITE);

        file->println("ez egy alma425");
        file->close();
    }

    t1.enable();
    Ethernet.init(PA9);

    while (true) {
        runner.execute();
        // nm->loop();
        serialEventRun();
    }

    return 0;
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