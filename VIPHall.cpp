#include "VIPHall.h"
#include <iostream>
#include <stdexcept>

VIPHall::VIPHall(int hallNumber, const Movie& currentMovie, int waitersCount)
    : Hall(hallNumber, currentMovie), waitersCount(waitersCount) {
    if (waitersCount < 0) throw std::invalid_argument("Waiters count cannot be negative.");
}

VIPHall::VIPHall(const VIPHall& other)
    : Hall(other), waitersCount(other.waitersCount) {}

VIPHall::~VIPHall() {}

int  VIPHall::getWaitersCount() const { return waitersCount; }

void VIPHall::setWaitersCount(int c) {
    if (c < 0) throw std::invalid_argument("Waiters count cannot be negative.");
    waitersCount = c;
}

void VIPHall::printHall() const {
    Hall::printHall();
    std::cout << "  [VIP Hall] Waiters on staff: " << waitersCount << std::endl;
}

Hall* VIPHall::clone() const {
    return new VIPHall(*this);
}
