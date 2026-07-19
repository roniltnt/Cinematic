#include "Hall.h"
#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: no Movie reference, no seats array (Deviations 1, 6)
Hall::Hall(int hallNumber) : hallNumber(hallNumber) {}

Hall::Hall(const Hall& other) : hallNumber(other.hallNumber) {}

Hall::~Hall() {}

int  Hall::getHallNumber() const { return hallNumber; }
void Hall::setHallNumber(int num) { hallNumber = num; }

// DEVIATION FROM ORIGINAL SPEC: virtual helpers — base returns defaults (Deviation 3, 8)
const char* Hall::getHallType()       const { return "Regular"; }
int         Hall::get3DGlassesCount() const { return 0; }
int         Hall::getWaitersCount()   const { return 0; }

void Hall::printHall() const {
    std::cout << "Hall #" << hallNumber << " [Regular]" << std::endl;
}

Hall* Hall::clone() const { return new Hall(*this); }
