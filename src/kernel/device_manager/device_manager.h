#ifndef DeviceManager_h
#define DeviceManager_h

#include <Arduino.h>
#include <stdlib.h>

#include <map>
#include <vector>

#include "../driver/drivers.h"
#include "../file_system/file_interface.h"
#include "../sysmacros.h"
#include "device_types.h"

class DeviceManager {
   public:
    DeviceManager(const DeviceManager&) = delete;

    DeviceManager& operator=(const DeviceManager&) = delete;

    static DeviceManager* getInstance();

    dev_t addDevice(DeviceTypes t_type, IFile* t_dev);
    IFile* getDevice(dev_t t_id);

    uint8_t removeDevice(dev_t t_id);

   private:
    inline static DeviceManager* m_instance{nullptr};
    std::map<dev_t, IFile*> m_devices;
    std::vector<int> m_device_record;
    DeviceManager() { m_device_record.assign(5, 0); }
    ~DeviceManager() = default;

    int exists(IFile* t_device_ptr);
};
#endif  // DeviceManager