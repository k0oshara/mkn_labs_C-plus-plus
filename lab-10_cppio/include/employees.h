#ifndef LAB10_EMPLOYEES_H_INCLUDED
#define LAB10_EMPLOYEES_H_INCLUDED

#include <vector>
#include <stdint.h>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>

namespace Employee {
    class Employee {
    public:
        Employee(std::string name = "", std::int32_t base_salary = 0);

        virtual ~Employee() = default;

        virtual int salary() const = 0;

        virtual void print_txt(std::ostream &out) const = 0;

        virtual void print_binary(std::ofstream &out) const = 0;

        virtual void read_txt(std::istream &in) = 0;

        virtual void read_binary(std::ifstream &in) = 0;

        friend std::istream &operator>>(std::istream &in, Employee &obj);

        friend std::ostream &operator<<(std::ostream &out, const Employee &obj);

        friend std::ifstream &operator>>(std::ifstream &in, Employee &obj);

        friend std::ofstream &operator<<(std::ofstream &out, const Employee &obj);

    protected:
        std::string _name;
        std::int32_t _base_salary;
    };

    class Developer : public Employee {
    public:
        Developer(std::string name = "", std::int32_t base_salary = 0, bool has_bonus = false);

        int salary() const override;

        void print_txt(std::ostream &out) const override;

        void print_binary(std::ofstream &out) const override;

        void read_txt(std::istream &in) override;

        void read_binary(std::ifstream &in) override;

    private:
        bool _has_bonus;
    };

    class SalesManager : public Employee {
    public:
        SalesManager(std::string name = "", std::int32_t base_salary = 0, std::int32_t sold_nm = 0, std::int32_t price = 0);

        int salary() const override;

        void print_txt(std::ostream &out) const override;

        void print_binary(std::ofstream &out) const override;

        void read_txt(std::istream &in) override;

        void read_binary(std::ifstream &in) override;

    private:
        std::int32_t _sold_nm, _price;
    };

    class EmployeesArray {
    public:
        EmployeesArray();

        ~EmployeesArray();

        int total_salary() const;

        void add(Employee *employee);

        friend std::istream &operator>>(std::istream &in, EmployeesArray &array);

        friend std::ostream &operator<<(std::ostream &out, const EmployeesArray &array);

        friend std::ifstream &operator>>(std::ifstream &in, EmployeesArray &array);

        friend std::ofstream &operator<<(std::ofstream &out, const EmployeesArray &array);

    private:
        std::vector<Employee*> _employees;
    };
}
#endif
