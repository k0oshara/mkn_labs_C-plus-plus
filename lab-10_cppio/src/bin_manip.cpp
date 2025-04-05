#include "bin_manip.h"

namespace bin_manip
{
    write_le_int32::write_le_int32( int32_t x) : _x(x) {}

    std::ostream &operator<<(std::ostream &out, write_le_int32 manip) {
        int32_t shift = 0;
        for (int32_t i = 0; i < 4; i++) {
            int32_t x = (manip._x >> shift) & 0xFF;
            out.put(x);
            shift+=8;
        }
        return out;
    }

    read_le_int32::read_le_int32(int32_t &x) : _x(&x) {}

    std::istream &operator>>(std::istream &in, read_le_int32 manip) {
        char bytes[4];
        in.read(bytes, 4);
        *manip._x = ((unsigned char)bytes[3] << 24) + ((unsigned char)bytes[2] << 16) + ((unsigned char)bytes[1] << 8) + (unsigned char)bytes[0];
        return in;
    }

    write_bool::write_bool(bool b) : _bool(b) {}

    std::ostream &operator<<(std::ostream &out, write_bool manip) {
        char b = manip._bool ? 1 : 0;
        out.write(&b, 1);
        return out;
    }

    read_bool::read_bool(bool &b) : _bool(&b) {}

    std::istream& operator>>(std::istream &in, read_bool manip) {
        in.read((char*)manip._bool, 1);
        return in;
    }

    write_c_str::write_c_str(char* str)
    {
        strcpy(_str, str);
    }

    std::ostream& operator<<(std::ostream &out, const write_c_str& manip) {
        out.write(manip._str, strlen(manip._str) + 1);
        return out;
    }


    read_c_str::read_c_str(char* str) : _str(str) {}


    std::istream& operator>>(std::istream &in, const read_c_str& manip) {
        size_t i = 0;
        while((in.get(manip._str[i])) && i < 102){
            if (manip._str[i] == '\0'){
                break;
            }
            i += 1;
        }
        if (i >= 102 && manip._str[i - 1] != '\0'){
            throw std::invalid_argument("Error: string too long");
        }
        return in;
    }

}
