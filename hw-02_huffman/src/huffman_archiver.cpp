#include "huffman_archiver.hpp"
#include "huffman_tree.hpp"
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

size_t initial_data = 0;
size_t size_add_inf = 0;
size_t size_compressed_data = 0;

namespace bin_manip{
    void write_bytes(std::ofstream &out, const size_t val){
        std::vector<char> val_bytes;
        for (size_t i = 0; i < sizeof(size_t); ++i) {
            char byte = (val >> (i * const_size_byte)) & const_lower_byte_mask;
            val_bytes.push_back(byte);
        }
        reverse(val_bytes.begin(), val_bytes.end());
        out.write(val_bytes.data(), val_bytes.size());
    }

    size_t read_bytes(std::ifstream &in){
        size_t val = 0;
        for (size_t j = 0; j < sizeof(size_t); j++) {
            char byte;
            in.get(byte);
            val <<= const_size_byte;
            val |= static_cast<size_t>(static_cast<unsigned char>(byte));
        }
        return val;
    }
}

void print(size_t arg1, size_t arg2, size_t arg3) {
    std::cout << arg1 << std::endl;
    std::cout << arg2 << std::endl;
    std::cout << arg3 << std::endl;
}

std::ifstream read_file_and_build_freq(const std::string &input_file, std::map<char, size_t> &freq){
    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open input file " + input_file);
    }
    char c;
    std::vector<char> str;
    while (in.get(c)) {
        str.push_back(c);
    }
    for (char i: str) {
        freq[i]++;
    }
    initial_data = str.size();
    return in;
}

std::ofstream freq_write_in_file(std::ifstream &in, const std::string &output_file, const std::map<char, size_t> &freq){
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Could not open output file " + output_file);
    }
    size_t freq_size = freq.size();
    size_add_inf = sizeof(freq_size);
    bin_manip::write_bytes(out, freq_size);
    for (const auto &pair: freq) {
        out.put(pair.first);
        size_add_inf += sizeof(pair.first);
        bin_manip::write_bytes(out, pair.second);
        size_add_inf += sizeof(pair.second);
    }
    in.clear();
    in.seekg(0);
    return out;
}

std::vector<bool> bit_buffer_table_codes (std::ifstream &in, tree Tree) {
    std::map<char, std::string> table = Tree.get_table_codes();
    std::vector<bool> bit_buffer;
    char c;
    while (in.get(c)) {
        for (char bit: table[c]) {
            bit_buffer.push_back(bit - '0');
        }
    }
    return bit_buffer;
}

void write_bit_buffer(std::ofstream &out, std::vector<bool> bit_buffer) {
    char byte = 0;
    int k = 0;
    size_compressed_data = 0;
    for (size_t i = 0; i < bit_buffer.size(); i++) {
        byte <<= 1;
        byte |= bit_buffer[i];
        k++;
        if (i % const_size_byte == const_size_byte - 1) {
            out.put(byte);
            size_compressed_data += sizeof(byte);
            byte = 0;
            k = 0;
        }
    }
    if (k > 0) {
        out.put(byte);
        size_compressed_data += sizeof(byte);
    }
}

std::ifstream read_freq(const std::string &input_file, std::map<char, size_t> &freq) {
    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open input file " + input_file);
    }
    size_t freq_size = bin_manip::read_bytes(in);
    size_add_inf = sizeof(freq_size);
    for (size_t i = 0; i < freq_size; i++) {
        char symbol;
        in.get(symbol);
        size_add_inf += sizeof(symbol);
        size_t fr = bin_manip::read_bytes(in);
        size_add_inf += sizeof(fr);
        freq[symbol] = fr;
    }
    return in;
}

std::ofstream decompress_text(std::ifstream &in, const std::string &output_file, tree Tree , tree::NodePtr root, int size_last_byte){
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Could not open output file " + output_file);
    }
    char byte;
    size_compressed_data = 0;
    initial_data = 0;
    while (in.get(byte)) {
        size_compressed_data += sizeof(byte);
        int size_proc_byte = 0;
        if (in.peek() == EOF){
            size_proc_byte = const_size_byte - size_last_byte;
        }
        for (int i = size_proc_byte; i < const_size_byte; i++) {
            unsigned char bit = (byte >> (const_size_byte - 1 - i)) & 1;
            if (bit == 0 && (root->get_node_left() != nullptr)) {
                root = root->get_node_left();
            }
            else if (bit == 1 && (root->get_node_right() != nullptr)){
                root = root->get_node_right();
            }
            if (root->get_node_left() == nullptr && root->get_node_right() == nullptr) {
                out.put(root->get_symbol());
                initial_data += sizeof(char);
                root = Tree.get_root();
            }
        }
    }
    return out;
}

void archiver::compress(const std::string &input_file, const std::string &output_file) {
    std::map<char, size_t> freq;
    std::ifstream in = read_file_and_build_freq(input_file, freq);
    tree Tree;
    Tree.build_tree(freq);
    std::ofstream out = freq_write_in_file(in, output_file, freq);
    std::vector<bool> bit_buffer = bit_buffer_table_codes(in, Tree);
    out.put(static_cast<char>(bit_buffer.size() % const_size_byte));
    size_add_inf += sizeof(char);
    write_bit_buffer(out, bit_buffer);
    print(initial_data, size_compressed_data, size_add_inf);
}

void archiver::decompress(const std::string &input_file, const std::string &output_file) {
    std::map<char, size_t> freq;
    std::ifstream in = read_freq(input_file, freq);
    char last_byte;
    in.get(last_byte);
    int size_last_byte = (static_cast<int>(last_byte) == 0) ? 8 :  static_cast<int>(last_byte);
    size_add_inf += sizeof(last_byte);
    tree Tree;
    Tree.build_tree(freq);
    tree::NodePtr root = Tree.get_root();
    std::map<char, std::string> tables = Tree.get_table_codes();
    std::ofstream out = decompress_text(in, output_file, Tree, root, size_last_byte);
    print(size_compressed_data, initial_data, size_add_inf);
}
