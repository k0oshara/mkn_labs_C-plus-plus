#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdint>

namespace bin_manip {
    class write_le_int32 {
    private:
        std::int32_t _x;
    public:
        write_le_int32(std::int32_t x);

        friend std::ostream &operator<<(std::ostream &out, write_le_int32 manip);
    };

    class read_le_int32 {
    private:
        std::int32_t* _x;
    public:
        read_le_int32(std::int32_t &x);

        friend std::istream &operator>>(std::istream &in, read_le_int32 manip);
    };


    class write_bool {
    private:
        bool _bool;
    public:
        write_bool(bool b);

        friend std::ostream &operator<<(std::ostream &out, write_bool manip);
    };

    class read_bool {
    private:
        bool* _bool;
    public:
        read_bool(bool &b);

        friend std::istream &operator>>(std::istream &in, read_bool manip);
    };


    class write_c_str {
    private:
        char _str[101];
    public:
        write_c_str(char* str);

        friend std::ostream &operator<<(std::ostream &out, const write_c_str &manip);
    };

    class read_c_str {
    private:
        char* _str;
    public:
        read_c_str(char* str);

        friend std::istream &operator>>(std::istream &in, const read_c_str &manip);
    };
}
