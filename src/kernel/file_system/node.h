#ifndef node_h
#define node_h

#include <Arduino.h>

namespace node {

enum Type { FOLDER, DEVICE };
struct Node {
    dev_t dev;
    Node* prev_node = nullptr;
    Type type = FOLDER;
    const char* name;
    std::vector<Node*> files;
};
} // namespace Node
#endif // node