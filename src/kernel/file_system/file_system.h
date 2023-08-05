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
#include "../device_manager/device_manager.h"
#include <file_system_interface.h>

using namespace node;
class FileSystem : public IFileSystem {
  public:
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

    static FileSystem* getInstance();
    String currentPath(Node* t_actualFolder);
    Node* search(const char* t_name, Type t_type, Node* t_actualFolder);
    uint8_t mknod(const char* t_path);

    uint8_t mknod(const char* t_path, dev_t t_dev);

    void rmnod(const char* t_path);

    uint8_t rmdir(const char* t_path);
    uint8_t rmdir(const String& t_path) { return this->rmdir(t_path.c_str()); };

    bool remove(const char* t_path) { return true; }
    bool remove(const String& t_path) { return this->remove(t_path.c_str()); };

    bool exists(const char* t_path) { return true; }
    bool exists(const String& t_path) { return this->exists(t_path.c_str()); };

    uint8_t mkdir(const char* t_path);
    uint8_t mkdir(const String& t_path) { return this->mkdir(t_path.c_str()); };
    IFile* open(const char* t_path, uint8_t t_mode = 1);
    IFile* open(const String& t_path, uint8_t t_mode = 1) {
        return this->open(t_path.c_str(), t_mode);
    };
    Node* lastFilePointer = &m_root;

  private:
    DeviceManager* m_dm_instance = DeviceManager::getInstance();
    uint8_t mknod(const char* t_path, Node* t_node);
    inline static FileSystem* m_instance{nullptr};
    Node m_root;
    FileSystem() {
        m_root.type = FOLDER;
        m_root.name = "";
    };
    ~FileSystem() = default;
};

#endif // FILESYSTEM_H
