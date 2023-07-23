#ifndef node_h
#define node_h

#include <Arduino.h>

enum nodeType { FOLDER, DEVICE };
struct node {
    dev_t dev;
    node* prevNode = nullptr;
    nodeType type = FOLDER;
    const char* name;
    std::vector<node*> files;
};
#endif  // node