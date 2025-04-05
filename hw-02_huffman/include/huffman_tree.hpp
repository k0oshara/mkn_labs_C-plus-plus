#pragma once

#include "huffman_node.hpp"
#include <map>

const std::string const_code_zero = "0";
const std::string const_code_one = "1";
const std::string const_empty_str = "";

class tree {
public:
    using NodePtr = Node::NodePtr;
private:
    NodePtr _root{nullptr};
    std::map<char, std::string> _table_codes;
public:
    struct compareNodes {
        bool operator()(NodePtr x, NodePtr y) const {
            return x->get_freq() > y->get_freq();
        }
    };
    void build_tree(const std::map<char, std::size_t> &freqs);
    void generate_codes(NodePtr node, std::string code);
    NodePtr get_root() const;
    std::map<char, std::string> get_table_codes() const;
};
