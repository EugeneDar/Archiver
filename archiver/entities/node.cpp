#include "node.h"

Node::Node(int frequency, int value) : frequency(frequency), value(value) {
    is_parent = false;
}
Node::Node(int frequency, int value, bool is_parent) : frequency(frequency), value(value), is_parent(is_parent) {
}