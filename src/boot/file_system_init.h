#ifndef file_system_init_h
#define file_system_init_h
#include <sysheaders.h>
#include <sysvar.h>
extern void fileSystemInit() {
    fileSystem.mkdir("/dev");
    fileSystem.mknod("/dev/rtc", deviceManager.addDevice(DeviceTypes::kRTC, &rtcdev));
    fileSystem.mknod("/dev/full", deviceManager.addDevice(DeviceTypes::kSystem, &fulldev));
    fileSystem.mknod("/dev/zero", deviceManager.addDevice(DeviceTypes::kSystem, &nulldev));
    fileSystem.mknod("/dev/null", deviceManager.addDevice(DeviceTypes::kSystem, &zerodev));
    fileSystem.mknod("/dev/tty", deviceManager.addDevice(DeviceTypes::kScreen, &term));
    fileSystem.mkdir("/user");
}
#endif // file_system_init