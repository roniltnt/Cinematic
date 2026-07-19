#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"
#include "Date.h"

class Employee : public Person {
private:
    Date   birthDate;
    double salary;

    // DEVIATION FROM ORIGINAL SPEC: static PROMOTION_PERCENTAGE removed (Deviation 9)

public:
    Employee(const char* name, int id, const Date& birthDate, double salary);
    Employee(const Employee& other);
    Employee& operator=(const Employee& other);
    ~Employee() override;

    const Date& getBirthDate() const;
    double      getSalary()    const;

    void setSalary(double newSalary);

    // DEVIATION FROM ORIGINAL SPEC: operator++() replaced by promote(pct).
    // User supplies the percentage at runtime instead of a hardcoded 10% (Deviation 9).
    void promote(double percentageIncrease);

    void printDetails() const override;
};

#endif
