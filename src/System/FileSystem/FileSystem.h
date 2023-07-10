#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <Streaming.h>

#include <map>
#include <vector>

#define PATH__MAX 64
#define NAME_MAX 32
#define SYMLOOP_MAX 10
#include "../DeviceManager/DeviceManager.h"
#include "../Utils/Device/Device.h"
// Commands
enum nodeType { FOLDER, DEVICE };
struct node {
    dev_t dev;
    node* prevNode = nullptr;
    nodeType type = FOLDER;
    const char* name;
    std::vector<node*> files;
};
class FileSystem {
   private:
    inline static FileSystem* instance{nullptr};
    node root;
    FileSystem() {
        root.type = FOLDER;
        root.name = "";
    };
    ~FileSystem() = default;
    DeviceManager* dmInstance = DeviceManager::getInstance();

    int mknod(const char* path, node* node);

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
    int mknod(const char* path);

    int mknod(const char* path, dev_t dev);

    void rmnod(const char* path);

    int rmdir(const char* path);

    int mkdir(const char* path);
    Device* open(const char* path);
    node* lastFilePointer = &root;
};

#endif  // FILESYSTEM_H
