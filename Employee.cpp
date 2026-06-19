#include "Employee.h"
#include <iostream>
#include <stdexcept>

const double Employee::PROMOTION_PERCENTAGE = 0.10;

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

Employee::~Employee() {}  // Person destructor handles the dynamic name

const Date& Employee::getBirthDate() const { return birthDate; }
double      Employee::getSalary()    const { return salary;    }

void Employee::setSalary(double newSalary) {
    if (newSalary < 0) throw std::invalid_argument("Salary cannot be negative.");
    salary = newSalary;
}

Employee& Employee::operator++() {
    salary *= (1.0 + PROMOTION_PERCENTAGE);
    return *this;
}

void Employee::printDetails() const {
    std::cout << "Employee: " << getName()
              << " | ID: "     << getId()
              << " | DOB: "    << birthDate
              << " | Salary: " << salary << std::endl;
}
