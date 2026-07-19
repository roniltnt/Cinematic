#include "Customer.h"
#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: clubPoints removed entirely (Deviation 1)
// DEVIATION FROM ORIGINAL SPEC: visitCount no longer a constructor parameter; auto-initialized to 0 (Deviation 2)

Customer::Customer(const char* name, int id)
    : Guest(name, id) {}

Customer::Customer(const Customer& other)
    : Guest(other) {}

Customer& Customer::operator=(const Customer& other) {
    if (this == &other) return *this;
    Guest::operator=(other);
    return *this;
}

void Customer::printDetails() const {
    // DEVIATION FROM ORIGINAL SPEC: clubPoints removed from output (Deviation 1)
    std::cout << "Customer: "  << getName()
              << " | ID: "     << getId()
              << " | Visits: " << getVisitCount()
              << " | Tickets: "<< getNumTickets() << std::endl;
}

Customer* Customer::clone() const {
    return new Customer(*this);
}
