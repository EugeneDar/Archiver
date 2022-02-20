#pragma once

#include <memory>

class Node {
public:
    int frequency;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    int value;
    bool is_parent;

    Node(int frequency, int value, bool is_parent);

    Node(int frequency, int value);
};
