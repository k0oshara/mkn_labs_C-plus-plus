#pragma once

#include <iostream>
#include <memory>

class Node {
public:
    using NodePtr = std::shared_ptr<Node>;
private:
    char _symbol;
    std::size_t _freq;
    NodePtr _left;
    NodePtr _right;
public:
    Node(char symbol = '0', std::size_t frequency = 0, NodePtr left = nullptr, NodePtr right = nullptr);
    std::size_t get_freq() const;
    NodePtr get_node_right() const;
    NodePtr get_node_left() const;
    char get_symbol() const;
    void set_node_right(NodePtr node);
    void set_node_left(NodePtr node);
};
