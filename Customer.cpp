#include "Customer.h"
#include <iostream>
#include <stdexcept>

Customer::Customer(const char* name, int id, int clubPoints, int visitCount)
    : Guest(name, id, visitCount), clubPoints(clubPoints) {
    if (clubPoints < 0) throw std::invalid_argument("Club points cannot be negative.");
}

Customer::Customer(const Customer& other)
    : Guest(other), clubPoints(other.clubPoints) {}

Customer& Customer::operator=(const Customer& other) {
    if (this == &other) return *this;
    Guest::operator=(other);
    clubPoints = other.clubPoints;
    return *this;
}

int  Customer::getClubPoints() const { return clubPoints; }

void Customer::setClubPoints(int p) {
    if (p < 0) throw std::invalid_argument("Club points cannot be negative.");
    clubPoints = p;
}

void Customer::addClubPoints(int p) {
    if (p < 0) throw std::invalid_argument("Points to add cannot be negative.");
    clubPoints += p;
}

void Customer::printDetails() const {
    std::cout << "Customer: "     << getName()
              << " | ID: "         << getId()
              << " | Visits: "     << getVisitCount()
              << " | Tickets: "    << getNumTickets()
              << " | Club pts: "   << clubPoints << std::endl;
}

Customer* Customer::clone() const {
    return new Customer(*this);
}
