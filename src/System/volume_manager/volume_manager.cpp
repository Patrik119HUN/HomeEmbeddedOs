#include "volume_manager.h"

VolumeManager* VolumeManager::GetInstance() {
    if (!instance) {
        instance = new VolumeManager();
    }
    return instance;
}
void VolumeManager::Mount(const char* name, FSType type, IFileSystem* fsptr) {
    VolumeInformation* vi = this->Search(name);
    if (vi != nullptr) return;
    VolumeInformation* _volume = new VolumeInformation{name, type, fsptr};
    _volume->print();
    volumes_.push_back(_volume);
}
void VolumeManager::UnMount(const char* name) { this->Search(name); }

IFileSystem* VolumeManager::GetVolume(const char* name) {
    VolumeInformation* vi = this->Search(name);
    if (vi == nullptr) return nullptr;
    return vi->fs;
}

VolumeInformation* VolumeManager::Search(const char* name) {
    for (VolumeInformation* i : volumes_) {
        if (strcmp(i->root_path_name, name) == 0) {
            return i;
        }
    }
    return nullptr;
}