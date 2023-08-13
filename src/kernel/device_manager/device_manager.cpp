#include "device_manager.h"

dev_t DeviceManager::addDevice(DeviceTypes t_type, IFile* t_dev) {
    dev_t a = m_device_record.at(t_type);
    dev_t id = makedev(t_type, a);
    m_devices.insert(std::pair{id, t_dev});
    m_device_record[t_type]++;
    return id;
};

IFile* DeviceManager::getDevice(dev_t t_id) { return m_devices.at(t_id); }

uint8_t DeviceManager::removeDevice(dev_t t_id) {
    m_devices.erase(t_id);
    return 1;
}
int DeviceManager::exists(IFile* t_device_ptr) {
    for (auto i : m_devices) {
        if (i.second == t_device_ptr) return 1;
    }
    return 0;
}
DeviceManager deviceManager;
