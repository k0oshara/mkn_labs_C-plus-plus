#include "matrix.h"
#include <iostream>
#include <vector>

int main() {
    const int size_registers = 10;

    std::vector<matrix::Matrix> registers = std::vector<matrix::Matrix>(size_registers);
    int row, col, number_register, number_register1, number_register2;

    char dollar = '$';

    std::string command, filename;

    while (true)
    {
        std::cin >> command;

        if (command == "exit")
        {
            return 0;
        }
        else if (command == "load")
        {
            std::cin >> dollar >> number_register >> filename;
            try
            {
                registers[number_register] = matrix::Matrix(filename);
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << e.what() << std::endl;
            }
            catch (std::bad_alloc &e)
            {
                std::cout << "Unable to allocate memory." << std::endl;
            }
        }
        else if (command == "print")
        {
            std::cin >> dollar >> number_register;
            registers[number_register].print(std::cout);
        }
        else if (command == "add")
        {
            std::cin >> dollar >> number_register1 >> dollar >> number_register2;
            try
            {
                registers[number_register1] += registers[number_register2];
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << e.what() << std::endl;
            }
        }
        else if (command == "mul")
        {
            std::cin >> dollar >> number_register1 >> dollar >> number_register2;
            try
            {
                registers[number_register1] *= registers[number_register2];
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << e.what() << std::endl;
            }
            catch (std::bad_alloc &e)
            {
                std::cout << "Unable to allocate memory." << std::endl;
            }
        }
        else if (command == "elem")
        {
            std::cin >> dollar >> number_register >> row >> col;
            try
            {
                std::cout << registers[number_register].get(row, col) << std::endl;
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << e.what() << std::endl;
            }
        }
    }
}
