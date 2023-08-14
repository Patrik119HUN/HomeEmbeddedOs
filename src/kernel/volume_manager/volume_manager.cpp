#include "volume_manager.h"

void VolumeManager::mount(const char* t_name, FSType t_type, IFileSystem* t_fsptr) {
    VolumeInformation* vi = this->search(t_name);
    if (vi != nullptr) return;
    VolumeInformation* _volume = new VolumeInformation{t_name, t_type, t_fsptr};
    m_volumes.push_back(_volume);
}
void VolumeManager::unMount(const char* t_name) { this->search(t_name); }

IFileSystem* VolumeManager::getVolume(const char* t_name) {
    VolumeInformation* vi = this->search(t_name);
    if (vi == nullptr) return nullptr;
    return vi->fs;
}

VolumeInformation* VolumeManager::search(const char* t_name) {
    for (VolumeInformation* i : m_volumes) {
        if (strcmp(i->root_path_name, t_name) == 0) {
            return i;
        }
    }
    return nullptr;
}
VolumeManager volumeManager;