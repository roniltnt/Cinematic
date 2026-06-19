#include "Hall3DVIP.h"
#include <iostream>

// With virtual inheritance, Hall3DVIP must explicitly call the Hall base constructor.
// VIPHall and Hall3D also list Hall(...) in their init lists, but those calls are
// suppressed by the compiler when the most-derived class (Hall3DVIP) is being built.
Hall3DVIP::Hall3DVIP(int hallNumber, const Movie& currentMovie, int waitersCount, int glassesCount)
    : Hall(hallNumber, currentMovie),
      VIPHall(hallNumber, currentMovie, waitersCount),
      Hall3D(hallNumber, currentMovie, glassesCount) {}

// Virtual base is initialized first from this copy ctor; VIPHall/Hall3D supply only
// their own extra fields (waitersCount, glassesCount).
Hall3DVIP::Hall3DVIP(const Hall3DVIP& other)
    : Hall(other),
      VIPHall(other),
      Hall3D(other) {}

Hall3DVIP::~Hall3DVIP() {}

void Hall3DVIP::printHall() const {
    Hall::printHall();
    std::cout << "  [3D+VIP Hall] Waiters: " << getWaitersCount()
              << " | Glasses: "               << getGlassesCount() << std::endl;
}

Hall* Hall3DVIP::clone() const {
    return new Hall3DVIP(*this);
}
