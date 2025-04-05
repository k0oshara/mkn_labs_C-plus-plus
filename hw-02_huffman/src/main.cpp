#include "huffman_archiver.hpp"
#include <cstring>

int main(int args, char* argv[]){
    if (args != 6 ){
        return -1;
    }
    std::string command;
    std::string in_file;
    std::string out_file;
    for (int i = 0; i < args; i++){
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-u") == 0){
            command = argv[i];
        }
        else if (strcmp(argv[i], "-f") == 0){
            in_file = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0){
            out_file = argv[++i];
        }
    }
    archiver Archiver;
    if (command == "-c"){
        Archiver.compress(in_file, out_file);
    }
    else if (command == "-u"){
        Archiver.decompress(in_file, out_file);
    }
    return 0;
}
