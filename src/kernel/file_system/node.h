#pragma once
#include <Arduino.h>

namespace node {

enum class Type { FOLDER, DEVICE };
struct Node {
    dev_t dev;
    Node* prev_node = nullptr;
    Type type = Type::FOLDER;
    const char* name;
    std::vector<Node*> files;
};
} // namespace Node