#ifndef VolumeManager_h
#define VolumeManager_h

#include <Arduino.h>
#include <Streaming.h>
#include <UUID.h>

#include <vector>

#include "../FileSystem/IFileSystem.h"
enum FSType { internal, FAT32, FAT16, exFAT };

struct VolumeInformation {
    const char* rootPathName;
    FSType FileSystemType;
    IFileSystem* fs;
    void print() { Serial << rootPathName << " " << FileSystemType << endl; }
};
class VolumeManager {
   private:
    inline static VolumeManager* instance{nullptr};
    std::vector<VolumeInformation*> vols;
    VolumeInformation* srch(const char* name) {
        for (VolumeInformation* i : vols) {
            if (strcmp(i->rootPathName, name) == 0) {
                return i;
            }
        }
        return nullptr;
    }
    VolumeManager() = default;
    ~VolumeManager() = default;

   public:
    VolumeManager(const VolumeManager&) = delete;
    VolumeManager& operator=(const VolumeManager&) = delete;
    static VolumeManager* getInstance() {
        if (!instance) {
            instance = new VolumeManager();
        }
        return instance;
    }

    void mount(const char* name, FSType type, IFileSystem* fsptr) {
        VolumeInformation* vi = this->srch(name);
        if (vi != nullptr) return;
        VolumeInformation* _vol = new VolumeInformation{name, type, fsptr};
        _vol->print();
        vols.push_back(_vol);
    }

    void unmount(const char* name) { this->srch(name); }

    IFileSystem* getVolume(const char* name) {
        VolumeInformation* vi = this->srch(name);
        if (vi == nullptr) return nullptr;
        return vi->fs;
    }
};

#endif  // VolumeManager