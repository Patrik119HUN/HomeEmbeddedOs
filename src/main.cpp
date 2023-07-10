#include <SimpleCLI.h>
#include <string.h>
#include <sysheaders.h>

#include "UUID.h"

FileSystem* fs = FileSystem::getInstance();

void sumCallback(cmd* c) {
    Command cmd(c);  // Create wrapper object

    int argNum = cmd.countArgs();  // Get number of arguments
    int sum = 0;

    // Go through all arguments and add their value up
    for (int i = 0; i < argNum; i++) {
        Argument arg = cmd.getArg(i);
        String argValue = arg.getValue();
        int argIntValue = argValue.toInt();

        if (argIntValue > 0) {
            if (i > 0) Serial.print('+');
            Serial.print(argIntValue);

            sum += argIntValue;
        }
    }
    // Print result
    Serial.print(" = ");
    Serial.println(sum);
}

UUID uuid;
DeviceManager* dv = DeviceManager::getInstance();
NetworkManager* nm = NetworkManager::getInstance();
// DigitalOutput relay;
// rtc* fasz = NULL;
// Screen* fos = NULL;
int main(void) {
    init();
    initVariant();
    randomSeed(analogRead(A0));
    uuid.seed(random(311), random(211));
    uuid.generate();
    fs->mkdir("/dev");
    fs->mkdir("/user");
    Serial.begin(9600);
    Ethernet.init(PA9);

    // dev_t rtcid = dv->addDev(DeviceManager::devTypes::rtcdev);
    // dev_t screenid = dv->addDev(DeviceManager::devTypes::screen);
    // fs->mkdir("/dev");
    // fs->mknod("/dev/rtc", rtcid);
    // fs->mknod("/dev/screen", screenid);
    // fasz = static_cast<rtc*>(fs->open("/dev/rtc"));
    // fos = static_cast<Screen*>(fs->open("/dev/screen"));
    // setDebugMessageLevel(DBG_INFO);

    while (true) {
        //nm->loop();
        // fos->ioctl(Screen::CTRLCMD::CLEAR, 0);
        // fos->write(fasz->read());
        serialEventRun();
    }

    return 0;
}