#include "Ticket.h"
#include "Screening.h"
#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: Ticket now references a Screening, not a Movie (Deviation 5).
// BASE_PRICE, THREE_D_SURCHARGE, calcFinalPrice(), operator> all removed (Deviation 4).

Ticket::Ticket(const Screening& screening, bool is3D)
    : screeningRef(screening), is3D(is3D) {}

// Copy binds to the same Screening (reference aliasing — Screening is owned by Cinema)
Ticket::Ticket(const Ticket& other)
    : screeningRef(other.screeningRef), is3D(other.is3D) {}

Ticket::~Ticket() {}

const Screening& Ticket::getScreening() const { return screeningRef; }
bool             Ticket::getIs3D()      const { return is3D;         }
void             Ticket::setIs3D(bool flag)    { is3D = flag;         }

// CHANGED: printTicket() replaces calcFinalPrice()
void Ticket::printTicket() const {
    std::cout << "[Regular] Movie: \"" << screeningRef.getMovie().getTitle() << "\""
              << " | Hall: #"          << screeningRef.getHall().getHallNumber()
              << " | Date: "           << screeningRef.getDate()
              << " | 3D: "             << (is3D ? "Yes" : "No")
              << std::endl;
}

Ticket* Ticket::clone() const { return new Ticket(*this); }
