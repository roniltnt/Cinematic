#include "Hall3D.h"
#include <iostream>
#include <stdexcept>

Hall3D::Hall3D(int hallNumber, const Movie& currentMovie, int glassesCount)
    : Hall(hallNumber, currentMovie), glassesCount(glassesCount) {
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

void Hall3D::printHall() const {
    Hall::printHall();
    std::cout << "  [3D Hall] Glasses available: " << glassesCount << std::endl;
}

Hall* Hall3D::clone() const {
    return new Hall3D(*this);
}
