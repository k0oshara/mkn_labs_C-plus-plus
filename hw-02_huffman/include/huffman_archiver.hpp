#pragma once

#include <string>

const int const_size_byte = 8;
const int const_lower_byte_mask = 0xFF;

class archiver {
public:
    void compress(const std::string& input_file, const std::string& output_file);
    void decompress(const std::string& input_file, const std::string& output_file);
};

namespace bin_manip {
    void write_bytes(std::ofstream &out, const size_t val);
    size_t read_bytes(std::ifstream &in);
}
