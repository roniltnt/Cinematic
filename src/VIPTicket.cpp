#include "../include/VIPTicket.h"
#include "../include/Screening.h"
#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: VIP_SURCHARGE, MEAL_PRICE, calcFinalPrice() removed (Deviation 4).
// Constructor now takes Screening reference (Deviation 5).

VIPTicket::VIPTicket(const Screening& screening, bool is3D, bool includesMeal)
    : Ticket(screening, is3D), includesMeal(includesMeal) {}

VIPTicket::VIPTicket(const VIPTicket& other)
    : Ticket(other), includesMeal(other.includesMeal) {}

VIPTicket::~VIPTicket() {}

bool VIPTicket::getIncludesMeal() const { return includesMeal; }
void VIPTicket::setIncludesMeal(bool m)  { includesMeal = m;   }

// CHANGED: printTicket() replaces calcFinalPrice()
void VIPTicket::printTicket() const {
    std::cout << "[VIP]     Movie: \"" << getScreening().getMovie().getTitle() << "\""
              << " | Hall: #"          << getScreening().getHall().getHallNumber()
              << " | Date: "           << getScreening().getDate()
              << " | 3D: "             << (getIs3D() ? "Yes" : "No")
              << " | Meal: "           << (includesMeal ? "Yes" : "No")
              << std::endl;
}

VIPTicket* VIPTicket::clone() const { return new VIPTicket(*this); }
