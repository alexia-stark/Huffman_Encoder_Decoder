#include "Node.h"

uint8_t Node::get_symbol() const {
    return this->symbol;
}
uint64_t Node::get_frequency() const{
    return this->frequency;
}
Node* Node::get_left() const {
    return this->left;
}
Node* Node::get_right() const {
    return this->right;
}

void Node::set_symbol(uint8_t s) {
    this->symbol = s;
}
void Node::set_frequency(uint64_t f) {
    this->frequency = f;
}
void Node::set_left(Node* l) {
    this->left = l;
}
void Node::set_right(Node* r) {
    this->right = r;
}

Node::Node(uint8_t given_symbol, uint64_t given_frequency) {
    this->left = nullptr;
    this->right = nullptr;
    this->symbol = given_symbol;
    this->frequency = given_frequency;
}

Node* create_parent(Node& left_child, Node& right_child) {
    uint64_t parent_frequency = left_child.frequency + right_child.frequency;
    Node* parent = new Node('$', parent_frequency);
    parent->set_left(&left_child);
    parent->set_right(&right_child);
    return parent;
}

bool Node::is_Leaf() {
    if (this->left == nullptr && this->right == nullptr)
        return 1;
    return 0;
}