#include "huffman_node.hpp"
#include <utility>

Node::Node(char symbol, std::size_t freq, NodePtr left, NodePtr right) : _symbol(symbol), _freq(freq), _left(std::move(left)), _right(std::move(right)) {}

std::size_t Node::get_freq() const {
    return _freq;
}

void Node::set_node_left(NodePtr left) {
    _left = left;
}

void Node::set_node_right(NodePtr right) {
    _right = right;
}

Node::NodePtr Node::get_node_right() const {
    return _right;
}

Node::NodePtr Node::get_node_left() const {
    return _left;
}

char Node::get_symbol() const {
    return _symbol;
}
