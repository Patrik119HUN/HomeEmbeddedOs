#ifndef DeviceManager_h
#define DeviceManager_h

#include <Arduino.h>
#include <stdlib.h>

#include <map>
#include <vector>

#include "../Driver/drivers.h"
#include "../Utils/Device/Device.h"
#include "../sysmacros.h"

class DeviceManager {
   private:
    inline static DeviceManager* instance{nullptr};
    std::map<dev_t, Device*> devices;
    std::vector<int> deviceRecord;
    DeviceManager() { deviceRecord.assign(5, 0); }
    ~DeviceManager() = default;

    int exits(Device* devPtr) {
        for (auto i : devices) {
            if (i.second == devPtr) return 1;
        }
        return 0;
    }

   public:
    DeviceManager(const DeviceManager&) = delete;

    DeviceManager& operator=(const DeviceManager&) = delete;

    static DeviceManager* getInstance() {
        if (!instance) {
            instance = new DeviceManager();
        }
        return instance;
    }

    enum devTypes { screen, rtcdev, relay, sd, sys };
    dev_t addDev(devTypes type, Device* dev) {
        dev_t a = deviceRecord.at(type);
        dev_t id = makedev(type, a);
        devices.insert(std::pair{id, dev});
        deviceRecord[type]++;
        return id;
    };

    Device* getDev(dev_t id) { return devices.at(id); }

    uint8_t rmdev(dev_t id) {
        devices.erase(id);
        return 1;
    }
};
#endif  // DeviceManager