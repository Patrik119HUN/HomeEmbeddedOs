#ifndef DeviceManager_h
#define DeviceManager_h

#include <Arduino.h>
#include <stdlib.h>

#include <map>
#include <vector>

#include "../Driver/drivers.h"
#include "../FileSystem/IFile.h"
#include "../sysmacros.h"
#include "device_types.h"

class DeviceManager {
   public:
    DeviceManager(const DeviceManager&) = delete;

    DeviceManager& operator=(const DeviceManager&) = delete;

    static DeviceManager* GetInstance();

    dev_t AddDevice(DeviceTypes type, IFile* dev);
    IFile* GetDevice(dev_t id);

    uint8_t RemoveDevice(dev_t id);

   private:
    inline static DeviceManager* instance{nullptr};
    std::map<dev_t, IFile*> devices_;
    std::vector<int> device_record_;
    DeviceManager() { device_record_.assign(5, 0); }
    ~DeviceManager() = default;

    int Exists(IFile* devPtr);
};
#endif  // DeviceManager