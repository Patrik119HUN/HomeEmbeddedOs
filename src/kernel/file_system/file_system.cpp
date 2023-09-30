#include "file_system.h"

Node* FileSystem::search(const char* t_name, Type t_type, Node* t_actualFolder) {
    std::vector<Node*> vec = t_actualFolder->files;
    for (auto i : vec) {
        if (i->type == t_type && strcmp(i->name, t_name) == 0) {
            return i;
        }
    }
    return nullptr;
}

uint8_t FileSystem::mkdir(const char* t_path) {
    if (strlen(t_path) <= 0) return ENOENT;
    if (strlen(t_path) > PATH__MAX) return ENAMETOOLONG;
    char* _path = strdup(t_path);
    std::vector<char*> paths = tokenize(_path, "/");

    if (paths.size() > SYMLOOP_MAX) return ELOOP;
    for (auto i : paths)
        if (strlen(i) > NAME_MAX_) return ENAMETOOLONG;

    Node* lastInstance = (_path[0] == '/') ? &m_root : lastFilePointer;

    while (!paths.empty()) {
        Node* srchres = this->search(paths.front(), Type::FOLDER, lastInstance);
        if (srchres != nullptr) {
            lastInstance = srchres;
        } else {
            Node* newFolder = new Node;
            newFolder->name = paths.front();
            newFolder->prev_node = lastInstance;
            lastInstance->files.push_back(newFolder);
            lastInstance = newFolder;
        }
        paths.erase(paths.begin());
    }
    return 1;
}

uint8_t FileSystem::rmdir(const char* t_path) {
    std::vector<char*> paths = tokenize(t_path, "/");

    Node* folder;
    char* _path = strdup(t_path);
    Node* lastInstance = (_path[0] == '/') ? &m_root : lastFilePointer;

    while (paths.size() != 1) {
        Node* srchres = this->search(paths.front(), Type::FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.erase(paths.begin());
    }

    folder = this->search(paths.front(), Type::FOLDER, lastInstance);
    paths.erase(paths.begin());
    if (!folder->files.empty()) return 0;
    auto it = std::find(lastInstance->files.begin(), lastInstance->files.end(), folder);
    lastInstance->files.erase(it);
    return 1;
}

void FileSystem::rmnod(const char* t_path) {}

uint8_t FileSystem::mknod(const char* t_path, Node* t_file) {
    char* _path = strdup(t_path);
    Node* lastInstance = (_path[0] == '/') ? &m_root : lastFilePointer;
    std::vector<char*> paths = tokenize(t_path, "/");
    t_file->name = paths.back();
    paths.pop_back();

    while (!paths.empty()) {
        Node* srchres = this->search(paths.front(), Type::FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.erase(paths.begin());
    }
    if (this->search(t_file->name, t_file->type, lastInstance) != nullptr) return 0;
    lastInstance->files.push_back(t_file);
    return 1;
}

String FileSystem::currentPath(Node* t_actualFolder) {
    String path = "";
    std::vector<String> pathNames;
    Node* lastInstance = t_actualFolder;
    pathNames.push_back(lastInstance->name);
    while (true) {
        if (lastInstance->prev_node == nullptr) break;
        lastInstance = lastInstance->prev_node;
        pathNames.push_back(lastInstance->name);
    }
    while (!pathNames.empty()) {
        path += pathNames.back();
        pathNames.pop_back();
        path += "/";
    }
    return path;
}

uint8_t FileSystem::mknod(const char* t_path, int t_dev) {
    Node* file = new Node;
    file->type = Type::DEVICE;
    file->dev = t_dev;
    return this->mknod(t_path, file);
}

uint8_t FileSystem::mknod(const char* t_path) {
    Node* file = new Node;
    return this->mknod(t_path, file);
}

IFile* FileSystem::open(const char* t_path, uint8_t t_mode) {
    char* _path = strdup(t_path);
    Node* lastInstance = (_path[0] == '/') ? &m_root : lastFilePointer;
    IFile* dev;
    std::vector<char*> paths = tokenize(t_path, "/");
    char* devName = paths.back();
    paths.pop_back();

    while (!paths.empty()) {
        Node* srchres = this->search(paths.front(), Type::FOLDER, lastInstance);
        if (srchres == nullptr) return 0;
        lastInstance = srchres;
        paths.erase(paths.begin());
    }
    Node* devFile = this->search(devName, Type::DEVICE, lastInstance);
    if (devFile == nullptr) return nullptr;
    //dev = deviceManager.getDevice("null");
    dev = nullptr;
    return dev;
}
FileSystem fileSystem;