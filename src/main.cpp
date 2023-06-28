#include <Arduino.h>
#include <Streaming.h>
#include <System/DeviceManager/DeviceManager.h>
#include <System/FileSystem/FileSystem.h>
#include <System/drivers.h>

DeviceManager* dv = DeviceManager::getInstance();
FileSystem* fs = FileSystem::getInstance();

DigitalOutput relay;
rtc* fasz = NULL;
Screen* fos = NULL;

void setup() {
    Serial.begin(9600);
    dev_t rtcid = dv->addDev(DeviceManager::devTypes::rtcdev);
    dev_t screenid = dv->addDev(DeviceManager::devTypes::screen);
    Serial << fs->mkdir("/dev") << endl;
    Serial << "Create rtc: " << fs->mknod("/dev/rtc", rtcid) << endl;
    Serial << "Create screen: " << fs->mknod("/dev/screen", screenid) << endl;
    fasz = static_cast<rtc*>(fs->open("/dev/rtc"));
    fos = static_cast<Screen*>(fs->open("/dev/screen"));
}
void loop() {
    relay.write(0b11111111);
    fos->ioctl(Screen::CTRLCMD::CLEAR, 0);
    fos->write(fasz->read());
    delay(500);
    relay.write(0b11000111);
    delay(500);
}