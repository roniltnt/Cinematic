#include "Screening.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdio>

// DEVIATION FROM ORIGINAL SPEC: New class — Movie+Hall scheduling unit (Deviation 2)
Screening::Screening(const Movie& movie, const Hall& hall, const Date& date,
                     int startHour, int startMinute, bool is3DScreening, int totalSeats)
    : movieRef(movie), hallRef(hall), screeningDate(date),
      startHour(startHour), startMinute(startMinute),
      is3DScreening(is3DScreening), totalSeats(totalSeats), soldTickets(0) {
    if (startHour < 0 || startHour > 23)   throw std::invalid_argument("Start hour must be 0-23.");
    if (startMinute < 0 || startMinute > 59) throw std::invalid_argument("Start minute must be 0-59.");
    if (totalSeats <= 0)                     throw std::invalid_argument("Total seats must be positive.");
}

// Copy constructor — both copies alias the same Movie and Hall objects
Screening::Screening(const Screening& other)
    : movieRef(other.movieRef), hallRef(other.hallRef),
      screeningDate(other.screeningDate),
      startHour(other.startHour), startMinute(other.startMinute),
      is3DScreening(other.is3DScreening),
      totalSeats(other.totalSeats), soldTickets(other.soldTickets) {}

Screening::~Screening() {}

const Movie& Screening::getMovie()          const { return movieRef;               }
const Hall&  Screening::getHall()           const { return hallRef;                }
const Date&  Screening::getDate()           const { return screeningDate;          }
int          Screening::getStartHour()      const { return startHour;              }
int          Screening::getStartMinute()    const { return startMinute;            }
bool         Screening::getIs3DScreening()  const { return is3DScreening;          }
int          Screening::getTotalSeats()     const { return totalSeats;             }
int          Screening::getSoldTickets()    const { return soldTickets;            }
int          Screening::getAvailableSeats() const { return totalSeats - soldTickets; }

int Screening::getStartTimeMinutes() const {
    return startHour * 60 + startMinute;
}

int Screening::getEndTimeMinutes() const {
    return getStartTimeMinutes() + movieRef.getLengthMinutes();
}

// DEVIATION FROM ORIGINAL SPEC: time-overlap detection for same hall+date (Deviation 2)
bool Screening::overlapsWith(const Screening& other) const {
    if (hallRef.getHallNumber() != other.hallRef.getHallNumber()) return false;
    if (!(screeningDate == other.screeningDate)) return false;
    // Half-open intervals [start, end) overlap when myStart < otherEnd && otherStart < myEnd
    return getStartTimeMinutes() < other.getEndTimeMinutes() &&
           other.getStartTimeMinutes() < getEndTimeMinutes();
}

// DEVIATION FROM ORIGINAL SPEC: decrements available seats (Deviation 6)
void Screening::sellSeat() {
    if (soldTickets >= totalSeats)
        throw std::runtime_error("No seats available for this screening.");
    ++soldTickets;
}

// DEVIATION FROM ORIGINAL SPEC: detailed print for table rows (Deviation 7)
void Screening::printScreening() const {
    char timeBuf[8];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d", startHour, startMinute);

    char seatsBuf[16];
    snprintf(seatsBuf, sizeof(seatsBuf), "%d/%d", soldTickets, totalSeats);

    std::cout << "Movie: \""   << movieRef.getTitle() << "\""
              << " | Hall: #"  << hallRef.getHallNumber()
              << " | Date: "   << screeningDate
              << " | Start: "  << timeBuf
              << " | 3D: "     << (is3DScreening ? "Yes" : "No ")
              << " | Sold: "   << seatsBuf
              << " | Free: "   << getAvailableSeats()
              << std::endl;
}
