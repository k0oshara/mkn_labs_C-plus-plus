#include "employees.h"
#include "bin_manip.h"

int main()
{
    Employee::EmployeesArray employees = Employee::EmployeesArray();
    while (true)
    {
        std::string input;
        std::cin >> input;
        if (input == "exit")
        {
            break;
        }

        if (input == "add")
        {
            std::cin >> employees;
        }
        if (input == "list")
        {
            std::cout << employees << std::endl;
        }
        if (input == "save")
        {
            std::string file_name;
            std::cin >> file_name;
            std::ofstream file;

            file.open(file_name, std::ios::binary);

            if (!file.is_open())
            {
                throw std::runtime_error("Can't open file");
            }

            file << employees;
        }
        if (input == "load")
        {
            std::string file_name;
            std::cin >> file_name;
            std::ifstream file;

            file.open(file_name, std::ios::binary);

            if (!file.is_open())
            {
                throw std::runtime_error("Can't open file");
            }

            file >> employees;
        }

    }
}
