#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "huffman_archiver.hpp"
#include "huffman_node.hpp"
#include "huffman_tree.hpp"
#include <sstream>
#include <string>

int check_indet_files(std::string input_file, std::string decompress_file) {
    std::ifstream in1(input_file);
    std::ifstream in2(decompress_file);
    CHECK(in1.is_open() == true);
    CHECK(in2.is_open() == true);
    char c;
    std::string str1, str2;
    while (in1.get(c)) {
        str1.append(reinterpret_cast<const char*>(&c), 1);
    }
    while (in2.get(c)) {
        str2.append(reinterpret_cast<const char*>(&c), 1);
    }
    return str1.compare(str2);
}

TEST_CASE("test_write_bytes_for_0...255"){
    std::string bin_file = "samples/test_bin_file.bin";
    std::vector<size_t> v{0, 1, 2, 5};
    std::ofstream out(bin_file, std::ios::binary);
    CHECK(out.is_open() == true);
    for (size_t i = 0; i < v.size(); i++){
        bin_manip::write_bytes(out, v[i]);
    }
    std::ifstream in(bin_file, std::ios::binary);
    CHECK(in.is_open() == true);
    char byte;
    size_t j = 0;
    size_t k = 0;
    while(in.get(byte)) {
        if (j != 0 && j % const_size_byte && k < v.size()) {
            CHECK(byte == v[k]);
            k++;
        }
        else {
            CHECK(byte == 0);
        }
        j++;
    }
}

TEST_CASE("test_write_bytes_for_0...256..."){
    std::string bin_file = "samples/test_bin_file.bin";
    std::vector<size_t> v{0, 1, 2, 5, 300, 1000, 256, 239};
    std::ofstream out(bin_file, std::ios::binary);
    CHECK(out.is_open() == true);
    for (size_t i = 0; i < v.size(); i++){
        bin_manip::write_bytes(out, v[i]);
    }
    out.close();
    std::ifstream in(bin_file, std::ios::binary);
    CHECK(in.is_open() == true);
    for (size_t i = 0; i < v.size(); i++) {
        size_t val = 0;
        for (size_t j = 0; j < sizeof(size_t); j++) {
            char byte;
            in.get(byte);
            val <<= const_size_byte;
            val |= static_cast<size_t>(static_cast<unsigned char>(byte));
        }
        CHECK(val == v[i]);
    }
}

TEST_CASE("test_read_bytes"){
    std::string bin_file = "samples/test_bin_file.bin";
    std::vector<size_t> v{0, 1, 2, 5, 300, 1000, 256, 239};
    std::ofstream out(bin_file, std::ios::binary);
    CHECK(out.is_open() == true);
    for (size_t i = 0; i < v.size(); i++){
        bin_manip::write_bytes(out, v[i]);
    }
    out.close();
    std::ifstream in(bin_file, std::ios::binary);
    in.seekg(0);
    CHECK(in.is_open() == true);
    for (size_t i = 0; i < v.size(); i++){
        CHECK(bin_manip::read_bytes(in) == v[i]);
    }

}

TEST_CASE("test_build_tree"){
    tree Tree;
    Tree.build_tree(std::map<char, size_t>{{'a', 3}, {'b', 1}, {'c', 1}});
    tree::NodePtr root = Tree.get_root();
    CHECK(root->get_node_right()->get_symbol() == 'a');
    CHECK(root->get_node_right()->get_freq() == 3);
    CHECK(root->get_node_left()->get_node_left()->get_symbol() == 'b');
    CHECK(root->get_node_left()->get_node_left()->get_freq() == 1);
    CHECK(root->get_node_left()->get_node_right()->get_symbol() == 'c');
    CHECK(root->get_node_left()->get_node_right()->get_freq() == 1);
}

TEST_CASE("test_generate_codes"){
    tree Tree;
    Tree.build_tree(std::map<char, size_t>{{'a', 3}, {'b', 1}, {'c', 1}});
    std::map<char, std::string> tables_code = Tree.get_table_codes();
    CHECK(tables_code['a'] == "1");
    CHECK(tables_code['b'] == "00");
    CHECK(tables_code['c'] == "01");
    Tree.build_tree(std::map<char, size_t>{{'a', 3}});
    std::map<char, std::string> tables_code_for_one = Tree.get_table_codes();
    CHECK(tables_code['a'] == "1");
}

TEST_CASE("test_compress_empty_file"){
    std::string input_file = "samples/test_empty_file_txt.txt", output_file = "samples/test_empty_file_bin.bin";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    std::ifstream in(output_file, std::ios::binary);
    CHECK(in.is_open() == true);
    size_t count_symbols = bin_manip::read_bytes(in);
    CHECK(count_symbols == 0);
    in.seekg(0);
    int size_file = 0;
    char c;
    while (in.get(c)){
        CHECK(c == 0);
        size_file += sizeof(char);
    }
    int check_size_file = sizeof(size_t) + sizeof(char);
    CHECK(size_file == check_size_file);
}

TEST_CASE("test_decompress_empty_file"){
    std::string input_file = "samples/test_empty_file_txt.txt", output_file = "samples/test_empty_file_bin.bin";
    std::string decompress_file = "samples/test_empty_file_decompress.txt";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    Archiver.decompress(output_file, decompress_file);
    CHECK(check_indet_files(input_file, decompress_file) == 0);
}

TEST_CASE("test_compress_one_symbols"){
    std::string input_file = "samples/test_one_symbol_file_txt.txt", output_file = "samples/test_one_symbol_file_bin.bin";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    std::ifstream in(output_file, std::ios::binary);
    CHECK(in.is_open() == true);
    size_t freq_size = bin_manip::read_bytes(in);
    CHECK(freq_size == 1);
    std::map<char, size_t> freq;
    char c;
    size_t freq_symbol = 0;
    for (size_t i = 0; i < freq_size; i++){
        in.get(c);
        freq_symbol = bin_manip::read_bytes(in);
        freq[c] = freq_symbol;
    }
    CHECK(freq.size() == 1);
    CHECK(freq['a'] == 1);
    tree Tree;
    Tree.build_tree(freq);
    std::map<char, std::string> tables_codes = Tree.get_table_codes();
    CHECK(tables_codes.size() == 1);
    CHECK(tables_codes['a'] == "0");
    char last_byte;
    in.get(last_byte);
    int size_last_byte = (static_cast<int>(last_byte) == 0) ? const_size_byte :  static_cast<int>(last_byte);
    CHECK(size_last_byte == 1);
    char cc;
    size_t size_str = 0;
    while(in.get(cc)){
        size_str += sizeof(cc);
    }
    CHECK(cc == 0);
    CHECK(size_str == 1);
}

TEST_CASE("test_decompress_one_symbols"){
    std::string input_file = "samples/test_one_symbol_file_txt.txt", output_file = "samples/test_one_symbol_file_bin.bin";
    std::string decompress_file = "samples/test_one_symbol_file_decompress.txt";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    Archiver.decompress(output_file, decompress_file);
    CHECK(check_indet_files(input_file, decompress_file) == 0);
}

TEST_CASE("test_compress_10_count_symbol_a"){
    std::string input_file = "samples/test_file_10_a.txt", output_file = "samples/test_file_10_a.bin";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    std::ifstream in(output_file, std::ios::binary);
    CHECK(in.is_open() == true);
    size_t freq_size = bin_manip::read_bytes(in);
    CHECK(freq_size == 1);
    std::map<char, size_t> freq;
    char c;
    size_t freq_symbol = 0;
    for (size_t i = 0; i < freq_size; i++){
        in.get(c);
        freq_symbol = bin_manip::read_bytes(in);
        freq[c] = freq_symbol;
    }
    CHECK(freq.size() == 1);
    tree Tree;
    Tree.build_tree(freq);
    std::map<char, std::string> tables_codes = Tree.get_table_codes();
    CHECK(tables_codes.size() == 1);
    char last_byte;
    in.get(last_byte);
    int size_last_byte = (static_cast<int>(last_byte) == 0) ? const_size_byte :  static_cast<int>(last_byte);
    CHECK(size_last_byte == 2);
    char cc;
    size_t size_str = 0;
    while(in.get(cc)){
        size_str += sizeof(cc);
    }
    CHECK(size_str == 2);
}

TEST_CASE("test_decompress_10_count_symbol_a"){
    std::string input_file = "samples/test_file_10_a.txt", output_file = "samples/test_file_10_a.bin";
    std::string decompress_file = "samples/test_decompress_file_10_a.txt";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    Archiver.decompress(output_file, decompress_file);
    CHECK(check_indet_files(input_file, decompress_file) == 0);
}

TEST_CASE("test_compress_count_symbols_256"){
    std::string input_file = "samples/test_file_256.txt", output_file = "samples/test_file_256.bin";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    std::ifstream in(output_file, std::ios::binary);
    CHECK(in.is_open() == true);
    size_t freq_size = bin_manip::read_bytes(in);
    CHECK(freq_size == 256);
    std::map<char, size_t> freq;
    char c;
    size_t freq_symbol = 0;
    for (size_t i = 0; i < freq_size; i++){
        in.get(c);
        freq_symbol = bin_manip::read_bytes(in);
        freq[c] = freq_symbol;
    }
    CHECK(freq.size() == 256);
    tree Tree;
    Tree.build_tree(freq);
    std::map<char, std::string> tables_codes = Tree.get_table_codes();
    CHECK(tables_codes.size() == 256);
    char last_byte;
    in.get(last_byte);
    int size_last_byte = (static_cast<int>(last_byte) == 0) ? const_size_byte :  static_cast<int>(last_byte);
    CHECK(size_last_byte == 8);
    char cc;
    size_t size_str = 0;
    while(in.get(cc)){
        size_str += sizeof(cc);
    }
    CHECK(size_str == 256);
}

TEST_CASE("test_decompress_count_symbols_256") {
    std::string input_file = "samples/test_file_256.txt", output_file = "samples/test_file_256.bin";
    std::string decompress_file = "samples/test_file_256_decompress.txt";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    Archiver.decompress(output_file, decompress_file);
    CHECK(check_indet_files(input_file, decompress_file) == 0);
}

TEST_CASE("test_decompress_file_Sherlock_Holmes"){
    std::string input_file = "samples/test_file_Sherlock_Holmes.txt", output_file = "samples/test_file_Sherlock_Holmes.bin";
    std::string decompress_file = "samples/test_file_decompress_Sherlock_Holmes.txt";
    archiver Archiver;
    Archiver.compress(input_file, output_file);
    Archiver.decompress(output_file, decompress_file);
    CHECK(check_indet_files(input_file, decompress_file) == 0);
}
