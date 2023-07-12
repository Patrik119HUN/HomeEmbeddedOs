#include <System/Driver/full/full.h>
#include <System/Driver/null/null.h>
#include <System/Driver/zero/zero.h>
#include <TaskScheduler.h>
#include <sysheaders.h>

Scheduler runner;
FileSystem* fs = FileSystem::getInstance();
console cli;

DeviceManager* dv = DeviceManager::getInstance();
NetworkManager* nm = NetworkManager::getInstance();

void cli_wrapper() { cli.loop(); }
Task t1(100 * TASK_MILLISECOND, TASK_FOREVER, &cli_wrapper, &runner);
full fulldev;
zero zerodev;
null nulldev;
Screen* scr;
int main(void) {
    init();
    initVariant();
    randomSeed(analogRead(A0));
    fs->mkdir("/dev");
    scr = new Screen;
    dev_t fullid = dv->addDev(DeviceManager::devTypes::sys, &fulldev);
    dev_t nullid = dv->addDev(DeviceManager::devTypes::sys, &nulldev);
    dev_t zeroid = dv->addDev(DeviceManager::devTypes::sys, &zerodev);
    dev_t scrid = dv->addDev(DeviceManager::devTypes::screen, scr);
    fs->mknod("/dev/full", fullid);
    fs->mknod("/dev/zero", nullid);
    fs->mknod("/dev/null", zeroid);
    fs->mknod("/dev/scr", scrid);
    fs->mkdir("/user");

    Device* dos = fs->open("/dev/scr");
    const char* szoveg = "alma";
    for (int i = 0; i < 4; i++) {
        dos->write(szoveg[i]);
    }
    Serial.begin(9600);
    t1.enable();
    Ethernet.init(PA9);

    while (true) {
        runner.execute();
        // nm->loop();
        serialEventRun();
    }

    return 0;
}