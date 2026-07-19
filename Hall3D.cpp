#include "Hall3D.h"
#include <iostream>
#include <stdexcept>

// DEVIATION FROM ORIGINAL SPEC: no Movie parameter (Deviation 1)
Hall3D::Hall3D(int hallNumber, int glassesCount)
    : Hall(hallNumber), glassesCount(glassesCount) {
    if (glassesCount < 0) throw std::invalid_argument("Glasses count cannot be negative.");
}

Hall3D::Hall3D(const Hall3D& other)
    : Hall(other), glassesCount(other.glassesCount) {}

Hall3D::~Hall3D() {}

int  Hall3D::getGlassesCount() const { return glassesCount; }

void Hall3D::setGlassesCount(int c) {
    if (c < 0) throw std::invalid_argument("Glasses count cannot be negative.");
    glassesCount = c;
}

// DEVIATION FROM ORIGINAL SPEC: provides 3D capacity info (Deviation 3)
const char* Hall3D::getHallType()       const { return "3D"; }
int         Hall3D::get3DGlassesCount() const { return glassesCount; }

void Hall3D::printHall() const {
    std::cout << "Hall #" << getHallNumber()
              << " [3D] | Glasses: " << glassesCount << std::endl;
}

Hall* Hall3D::clone() const { return new Hall3D(*this); }
