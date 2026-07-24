#include "../include/Hall3DVIP.h"
#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: no Movie parameter; virtual base Hall initialized directly (Deviation 1)
Hall3DVIP::Hall3DVIP(int hallNumber, int waitersCount, int glassesCount)
    : Hall(hallNumber),
      VIPHall(hallNumber, waitersCount),
      Hall3D(hallNumber, glassesCount) {}

// Virtual base Hall initialized first; VIPHall/Hall3D supply their own extra fields only
Hall3DVIP::Hall3DVIP(const Hall3DVIP& other)
    : Hall(other),
      VIPHall(other),
      Hall3D(other) {}

Hall3DVIP::~Hall3DVIP() {}

// DEVIATION FROM ORIGINAL SPEC: resolves getHallType() ambiguity from Hall3D/"3D" vs VIPHall/"VIP"
const char* Hall3DVIP::getHallType() const { return "3D+VIP"; }

void Hall3DVIP::printHall() const {
    std::cout << "Hall #" << getHallNumber()
              << " [3D+VIP] | Waiters: " << getWaitersCount()
              << " | Glasses: "           << getGlassesCount() << std::endl;
}

Hall* Hall3DVIP::clone() const { return new Hall3DVIP(*this); }
