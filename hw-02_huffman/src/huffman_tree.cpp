#include "huffman_tree.hpp"
#include <queue>

void tree::build_tree(const std::map<char, std::size_t> &freqs) {
    std::priority_queue<NodePtr, std::vector<NodePtr>, compareNodes> q;
    for (const auto &pair: freqs) {
        q.push(static_cast<std::shared_ptr<Node>>(new Node(pair.first, pair.second)));
    }
    while (q.size() > 1) {
        NodePtr left = q.top();
        q.pop();
        NodePtr right = q.top();
        q.pop();
        NodePtr parent = static_cast<std::shared_ptr<Node>>(new Node('\0', left->get_freq() + right->get_freq(), left,right));
        parent->set_node_left(left);
        parent->set_node_right(right);
        q.push(parent);
    }
    if (!q.empty())
    {
        _root = q.top();
        generate_codes(_root, const_empty_str);
    }
}

void tree::generate_codes(NodePtr node, std::string code) {
    if (node == nullptr) {
        return;
    }
    if (node->get_node_left() == nullptr && node->get_node_right() == nullptr) {
        if (node == _root) {
            _table_codes[node->get_symbol()] = const_code_zero;
        } else {
            _table_codes[node->get_symbol()] = code;
        }
        return;
    }
    generate_codes(node->get_node_left(), code + const_code_zero);
    generate_codes(node->get_node_right(), code + const_code_one);
}

std::map<char, std::string> tree::get_table_codes() const {
    return _table_codes;
}

tree::NodePtr tree::get_root() const{
    return _root;
}
