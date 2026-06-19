#include "Shift.h"
#include <iostream>
#include <stdexcept>

Shift::Shift(const Employee& worker, int shiftLength, const Date& shiftDate)
    : workerRef(worker), shiftLength(shiftLength), shiftDate(shiftDate) {
    if (shiftLength <= 0) throw std::invalid_argument("Shift length must be positive.");
}

// const Employee& cannot be reseated — we bind the copy to the same Employee object.
// Shift::operator= is deleted in the header precisely because of this reference member.
Shift::Shift(const Shift& other)
    : workerRef(other.workerRef), shiftLength(other.shiftLength), shiftDate(other.shiftDate) {}

Shift::~Shift() {}  // no owned dynamic memory

const Employee& Shift::getWorker()    const { return workerRef;   }
int             Shift::getShiftLength() const { return shiftLength; }
const Date&     Shift::getShiftDate() const { return shiftDate;   }

void Shift::setShiftLength(int hours) {
    if (hours <= 0) throw std::invalid_argument("Shift length must be positive.");
    shiftLength = hours;
}

void Shift::printShift() const {
    std::cout << "Shift on " << shiftDate
              << " | Duration: " << shiftLength << "h"
              << " | Employee: " << workerRef.getName() << std::endl;
}
