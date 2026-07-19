#include "Employee.h"
#include <iostream>
#include <stdexcept>

// DEVIATION FROM ORIGINAL SPEC: PROMOTION_PERCENTAGE constant removed (Deviation 9)

Employee::Employee(const char* name, int id, const Date& birthDate, double salary)
    : Person(name, id), birthDate(birthDate), salary(salary) {
    if (salary < 0) throw std::invalid_argument("Salary cannot be negative.");
}

Employee::Employee(const Employee& other)
    : Person(other), birthDate(other.birthDate), salary(other.salary) {}

Employee& Employee::operator=(const Employee& other) {
    if (this == &other) return *this;
    Person::operator=(other);
    birthDate = other.birthDate;
    salary    = other.salary;
    return *this;
}

Employee::~Employee() {}

const Date& Employee::getBirthDate() const { return birthDate; }
double      Employee::getSalary()    const { return salary;    }

void Employee::setSalary(double newSalary) {
    if (newSalary < 0) throw std::invalid_argument("Salary cannot be negative.");
    salary = newSalary;
}

// DEVIATION FROM ORIGINAL SPEC: user-supplied percentage instead of hardcoded 10% (Deviation 9)
void Employee::promote(double percentageIncrease) {
    if (percentageIncrease < 0)
        throw std::invalid_argument("Promotion percentage cannot be negative.");
    salary *= (1.0 + percentageIncrease / 100.0);
}

void Employee::printDetails() const {
    std::cout << "Employee: " << getName()
              << " | ID: "     << getId()
              << " | DOB: "    << birthDate
              << " | Salary: " << salary << std::endl;
}
