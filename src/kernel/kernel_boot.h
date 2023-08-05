#ifndef kernel_boot_h
#define kernel_boot_h

#include "sysvar.h"
#include <sysheaders.h>
extern void kernel_boot(void) {
    DeviceManager* dv = DeviceManager::getInstance();
    VolumeManager* vm = VolumeManager::getInstance();
    FileSystem* fs = FileSystem::getInstance();
    
    pinMode(PB13,OUTPUT);
    digitalWrite(PB13,HIGH);
    delay(100);
    digitalWrite(PB13,LOW);
    Serial.begin(BAUD_RATE);
    Ethernet.init(W5500_PIN);
    if (!SD.begin(SD_PIN)) {
        Serial.println("initialization failed!");
        while (1)
            ;
    }
    vm->mount("C", FSType::FAT32, &SD);
    fs->mkdir("/dev");
    fs->mknod("/dev/full", dv->addDevice(DeviceTypes::kSystem, &fulldev));
    fs->mknod("/dev/zero", dv->addDevice(DeviceTypes::kSystem, &nulldev));
    fs->mknod("/dev/null", dv->addDevice(DeviceTypes::kSystem, &zerodev));
    fs->mknod("/dev/tty", dv->addDevice(DeviceTypes::kScreen, &term));
    fs->mkdir("/user");
    t1.enable();
}
#endif // kernel_boot