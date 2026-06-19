#include "Hall.h"
#include <iostream>
#include <stdexcept>

Hall::Hall(int hallNumber, const Movie& currentMovie)
    : hallNumber(hallNumber), currentMovie(currentMovie) {
    for (int i = 0; i < NUM_SEATS; ++i)
        seats[i] = false;
}

// Both halls share the same Movie reference (reference aliasing by design)
Hall::Hall(const Hall& other)
    : hallNumber(other.hallNumber), currentMovie(other.currentMovie) {
    for (int i = 0; i < NUM_SEATS; ++i)
        seats[i] = other.seats[i];
}

Hall::~Hall() {}  // virtual; does not own the Movie it references

int          Hall::getHallNumber()    const { return hallNumber;    }
const Movie& Hall::getCurrentMovie()  const { return currentMovie;  }

bool Hall::isSeatTaken(int seatIndex) const {
    if (seatIndex < 0 || seatIndex >= NUM_SEATS)
        throw std::out_of_range("Seat index out of range.");
    return seats[seatIndex];
}

int Hall::countTakenSeats() const {
    int count = 0;
    for (int i = 0; i < NUM_SEATS; ++i)
        if (seats[i]) ++count;
    return count;
}

void Hall::setHallNumber(int num) { hallNumber = num; }

void Hall::takeSeat(int seatIndex) {
    if (seatIndex < 0 || seatIndex >= NUM_SEATS)
        throw std::out_of_range("Seat index out of range.");
    if (seats[seatIndex])
        throw std::runtime_error("Seat is already taken.");
    seats[seatIndex] = true;
}

void Hall::freeSeat(int seatIndex) {
    if (seatIndex < 0 || seatIndex >= NUM_SEATS)
        throw std::out_of_range("Seat index out of range.");
    seats[seatIndex] = false;
}

// Returns true when every seat is free (operator! = "is empty?")
bool Hall::operator!() const {
    return countTakenSeats() == 0;
}

void Hall::printHall() const {
    std::cout << "Hall #" << hallNumber
              << " | Movie: \"" << currentMovie.getTitle() << "\""
              << " | Seats taken: " << countTakenSeats() << "/" << NUM_SEATS << std::endl;
}

Hall* Hall::clone() const {
    return new Hall(*this);
}
