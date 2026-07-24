#ifndef SHIFT_H
#define SHIFT_H

#include "Employee.h"
#include "Date.h"

class Shift {
private:
    const Employee& workerRef;
    int shiftLength;
    Date shiftDate;

public:
    Shift(const Employee& worker, int shiftLength, const Date& shiftDate);
    Shift(const Shift& other);
    Shift& operator=(const Shift& other) = delete;
    ~Shift();

    const Employee& getWorker() const;
    int getShiftLength() const;
    const Date& getShiftDate() const;

    void setShiftLength(int hours);

    void printShift() const;
};

#endif
