#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <Streaming.h>

#include <map>
#include <vector>

#include "node.h"

#define PATH__MAX 64
#define NAME_MAX_ 32
#define SYMLOOP_MAX 10
#include "../DeviceManager/DeviceManager.h"
#include "IFileSystem.h"
class FileSystem : public IFileSystem {
   private:
    inline static FileSystem* instance{nullptr};
    node root;
    FileSystem() {
        root.type = FOLDER;
        root.name = "";
    };
    ~FileSystem() = default;
    DeviceManager* dmInstance = DeviceManager::getInstance();
    uint8_t mknod(const char* path, node* node);

   public:
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

    static FileSystem* getInstance() {
        if (!instance) {
            instance = new FileSystem();
        }
        return instance;
    }
    String currentPath(node* actualFolder);
    node* search(const char* name, nodeType type, node* actualFolder);
    uint8_t mknod(const char* path);

    uint8_t mknod(const char* path, dev_t dev);

    void rmnod(const char* path);

    uint8_t rmdir(const char* path);
    uint8_t rmdir(const String& path) { return this->rmdir(path.c_str()); };

    bool remove(const char* path) { return true; }
    bool remove(const String& path) { return this->remove(path.c_str()); };

    bool exists(const char* path) { return true; }
    bool exists(const String& path) { return this->exists(path.c_str()); };
    
    uint8_t mkdir(const char* path);
    uint8_t mkdir(const String& path) { return this->mkdir(path.c_str()); };
    IFile* open(const char* path, uint8_t mode = 1);
    IFile* open(const String& path, uint8_t mode = 1) { return this->open(path.c_str(), mode); };
    node* lastFilePointer = &root;
};

#endif  // FILESYSTEM_H
