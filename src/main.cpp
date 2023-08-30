#include "sysvar.h"

#include "kernel/kernel.h"

#include "program/cli_deamon.h"
#include "program/ntp_deamon.h"
#include <Arduino.h>

#include <ethernet_handler/ethernet_handler.h>

void setup() {
    Debug.timestampOn();
    Debug.setDebugLevel(DBG_VERBOSE);
    Serial.begin(9600);
    fileSystem.mkdir("/dev");
    fileSystem.mknod("/dev/rtc", deviceManager.addDevice(DeviceTypes::REAL_TIME_CLOCK, &rtcdev));
    fileSystem.mknod("/dev/full", deviceManager.addDevice(DeviceTypes::SYSTEM, &fulldev));
    //fileSystem.mknod("/dev/zero", deviceManager.addDevice(DeviceTypes::SYSTEM, &nulldev));
    //fileSystem.mknod("/dev/null", deviceManager.addDevice(DeviceTypes::SYSTEM, &zerodev));
    //fileSystem.mknod("/dev/tty", deviceManager.addDevice(DeviceTypes::SCREEN, &term));
    //fileSystem.mkdir("/user");
    processSettings cli{"cli_deamon", ProcessPriority::BASE};
   // processSettings ntp{"ntp_deamon", ProcessPriority::BASE};
    //processSettings network{"network_deamon", ProcessPriority::BASE};
    //processSettings dummy_settings{"dummy", ProcessPriority::BASE};
    tone(PB13, 2000, 100);
    Ethernet.init(W5500_PIN);
    if (!SD.begin(SD_PIN)) {
        DEBUG_ERROR("SD initialization failed");
        while (1)
            ;
    }
    volumeManager.mount("C", FSType::FAT32, &SD);

    EthernetConnectionHandler EthernetAdapter;

    networkManager.addAdapter(&EthernetAdapter);
    networkManager.setClient(&EthernetAdapter.getClient());
    networkManager.setUDP(&EthernetAdapter.getUDP());

    // processManager.startProcess(&ntp, &ntp_deamon, 0);
    processManager.startProcess(&cli, &cli_deamon, 0);
}
void loop() { processManager.loop(); }