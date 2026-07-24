#include "../include/VIPHall.h"
#include <iostream>
#include <stdexcept>

// DEVIATION FROM ORIGINAL SPEC: no Movie parameter (Deviation 1)
VIPHall::VIPHall(int hallNumber, int waitersCount)
    : Hall(hallNumber), waitersCount(waitersCount) {
    if (waitersCount < 0) throw std::invalid_argument("Waiters count cannot be negative.");
}

VIPHall::VIPHall(const VIPHall& other)
    : Hall(other), waitersCount(other.waitersCount) {}

VIPHall::~VIPHall() {}

// DEVIATION FROM ORIGINAL SPEC: satisfies Hall's virtual override (Deviation 8)
const char* VIPHall::getHallType()    const { return "VIP"; }
int         VIPHall::getWaitersCount() const { return waitersCount; }

void VIPHall::setWaitersCount(int c) {
    if (c < 0) throw std::invalid_argument("Waiters count cannot be negative.");
    waitersCount = c;
}

void VIPHall::printHall() const {
    std::cout << "Hall #" << getHallNumber()
              << " [VIP] | Waiters: " << waitersCount << std::endl;
}

Hall* VIPHall::clone() const { return new VIPHall(*this); }
