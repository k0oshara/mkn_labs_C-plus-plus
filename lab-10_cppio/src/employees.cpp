#include "employees.h"
#include "bin_manip.h"

namespace Employee {
    std::istream &operator>>(std::istream &in, Employee &obj) {
        obj.read_txt(in);
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const Employee &obj) {
        obj.print_txt(out);
        return out;
    }

    std::ifstream &operator>>(std::ifstream &in, Employee &obj) {
        obj.read_binary(in);
        return in;
    }

    std::ofstream &operator<<(std::ofstream &out, const Employee &obj) {
        obj.print_binary(out);
        return out;
    }


    Employee::Employee(std::string name, int32_t base_salary) : _name(name), _base_salary(base_salary) {}


    void Employee::print_txt(std::ostream &out) const
    {
        out << "Name: " << _name.c_str() << std::endl << "Base Salary: " << _base_salary << std::endl;
    }

    void Employee::print_binary(std::ofstream &out) const
    {
        out << bin_manip::write_c_str((char*)_name.c_str()) << bin_manip::write_le_int32(_base_salary);
    }

    void Employee::read_txt(std::istream &in)
    {
        in >> _name >> _base_salary;
    }
    void Employee::read_binary(std::ifstream &in)
    {
        char name[101];
        in >> bin_manip::read_c_str(name) >> bin_manip::read_le_int32(_base_salary);
        _name = name;
    }


    Developer::Developer(std::string name, int32_t base_salary, bool has_bonus) : Employee(name, base_salary), _has_bonus(has_bonus) {}

    int Developer::salary() const {
        int32_t salary = _base_salary;
        if (_has_bonus) {
            salary += 1000;
        }
        return salary;
    }


    void Developer::print_txt(std::ostream &out) const {
        char sing = _has_bonus ? '+' : '-';
        out << "Developer" << std::endl;
        Employee::print_txt(out);
        out << "Has bonus: " << sing;
    }

    void Developer::print_binary(std::ofstream &out) const {
        out << bin_manip::write_le_int32((int32_t) 1);
        Employee::print_binary(out);
        out << bin_manip::write_bool(_has_bonus);
    }

    void Developer::read_binary(std::ifstream &in) {
        Employee::read_binary(in);
        in >> bin_manip::read_bool(_has_bonus);
    }

    void Developer::read_txt(std::istream &in) {
        Employee::read_txt(in);
        in >> _has_bonus;
    }

    SalesManager::SalesManager(std::string name, int32_t base_salary, int32_t sold_nm, int32_t price) : Employee(name,base_salary), _sold_nm(sold_nm), _price(price) {}

    int SalesManager::salary() const {
        return _base_salary + _sold_nm * _price * 0.01;
    }

    void SalesManager::print_txt(std::ostream &out) const {
        out << "Sales Manager" << std::endl;
        Employee::print_txt(out);
        out << "Sold items: " << _sold_nm << std::endl << "Item price: " << _price;
    }

    void SalesManager::print_binary(std::ofstream &out) const {
        out << bin_manip::write_le_int32(2);
        Employee::print_binary(out);
        out << bin_manip::write_le_int32(_sold_nm) << bin_manip::write_le_int32(_price);
    }

    void SalesManager::read_binary(std::ifstream &in) {
        Employee::read_binary(in);
        in >> bin_manip::read_le_int32(_sold_nm) >> bin_manip::read_le_int32(_price);
    }

    void SalesManager::read_txt(std::istream &in) {
        Employee::read_txt(in);
        in >> _sold_nm >> _price;
    }

    EmployeesArray::EmployeesArray() : _employees(std::vector<Employee *>()) {}

    EmployeesArray::~EmployeesArray() {
        for (int32_t i = 0; i < (int32_t) _employees.size(); i++) {
            delete _employees[i];
        }
    }

    int EmployeesArray::total_salary() const {
        int32_t salary = 0;
        for (auto &e: _employees) {
            salary += e->salary();
        }
        return salary;
    }

    void EmployeesArray::add(Employee *employee) {
        _employees.push_back(employee);
    }

    std::istream &operator>>(std::istream &in, EmployeesArray &array) {
        int type;
        in >> type;
        Employee *employee;
        if (type == 1)
        {
            employee = new Developer();
            in >> *employee;
            array.add(employee);
        }
        else if (type == 2)
        {
            employee = new SalesManager();
            in >> *employee;
            array.add(employee);
        }
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const EmployeesArray &array) {
        for (size_t i = 0; i < array._employees.size(); i++) {
            out << i + 1 << ". " << *array._employees[i] << std::endl;
        }
        std::cout << "== Total salary: " << array.total_salary() << std::endl;
        return out;
    }

    std::ifstream &operator>>(std::ifstream &in, EmployeesArray &array) {
        int32_t size;

        in >> bin_manip::read_le_int32(size);

        if (!in)
        {
            throw std::runtime_error("Failed to read size from input stream");
        }

        for (int32_t i = 0; i < size; i++)
        {
            int32_t type;
            in >> bin_manip::read_le_int32(type);

            Employee *employee;
            if (type == 1)
            {
                employee = new Developer();
                in >> *employee;
                array.add(employee);
            }
            else if (type == 2)
            {
                employee = new SalesManager();
                in >> *employee;
                array.add(employee);
            }
        }
        return in;
    }

    std::ofstream &operator<<(std::ofstream &out, const EmployeesArray &array) {
        out << bin_manip::write_le_int32((std::int32_t) array._employees.size());
        for (size_t i = 0; i < array._employees.size(); i++) {
            out << *array._employees[i];
        }
        return out;
    }
}
