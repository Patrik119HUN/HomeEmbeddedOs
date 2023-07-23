#ifndef DeviceManager_h
#define DeviceManager_h

#include <Arduino.h>
#include <stdlib.h>

#include <map>
#include <vector>

#include "../Driver/drivers.h"
#include "../sysmacros.h"
#include "../FileSystem/IFile.h"
class DeviceManager {
   private:
    inline static DeviceManager* instance{nullptr};
    std::map<dev_t, IFile*> devices;
    std::vector<int> deviceRecord;
    DeviceManager() { deviceRecord.assign(5, 0); }
    ~DeviceManager() = default;

    int exits(IFile* devPtr) {
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
    dev_t addDev(devTypes type, IFile* dev) {
        dev_t a = deviceRecord.at(type);
        dev_t id = makedev(type, a);
        devices.insert(std::pair{id, dev});
        deviceRecord[type]++;
        return id;
    };

    IFile* getDev(dev_t id) { return devices.at(id); }

    uint8_t rmdev(dev_t id) {
        devices.erase(id);
        return 1;
    }
};
#endif  // DeviceManager