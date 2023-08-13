#ifndef DeviceManager_h
#define DeviceManager_h
#include <Arduino.h>
#include <stdlib.h>

#include <map>
#include <vector>

#include "../../driver/drivers.h"
#include "../sysmacros.h"
#include "device_types.h"
#include <file_interface.h>
class DeviceManager {
  public:
    DeviceManager() { m_device_record.assign(5, 0); }
    ~DeviceManager() = default;

  public:
    dev_t addDevice(DeviceTypes t_type, IFile* t_dev);
    IFile* getDevice(dev_t t_id);

    uint8_t removeDevice(dev_t t_id);

  private:
    std::map<dev_t, IFile*> m_devices;
    std::vector<int> m_device_record;

    int exists(IFile* t_device_ptr);
} ;
extern DeviceManager deviceManager;
#endif // DeviceManager