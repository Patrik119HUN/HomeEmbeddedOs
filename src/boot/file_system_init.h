#ifndef file_system_init_h
#define file_system_init_h
#include <sysheaders.h>
#include <sysvar.h>
extern void fileSystemInit() {
    FileSystem* fs = FileSystem::getInstance();
    DeviceManager* dv = DeviceManager::getInstance();
    fs->mkdir("/dev");
    fs->mknod("/dev/rtc", dv->addDevice(DeviceTypes::kRTC, &rtcdev));
    fs->mknod("/dev/full", dv->addDevice(DeviceTypes::kSystem, &fulldev));
    fs->mknod("/dev/zero", dv->addDevice(DeviceTypes::kSystem, &nulldev));
    fs->mknod("/dev/null", dv->addDevice(DeviceTypes::kSystem, &zerodev));
    fs->mknod("/dev/tty", dv->addDevice(DeviceTypes::kScreen, &term));
    fs->mkdir("/user");
}
#endif // file_system_init