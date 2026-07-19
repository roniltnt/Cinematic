#include "Guest.h"
#include "Ticket.h"
#include <iostream>
#include <stdexcept>

// DEVIATION FROM ORIGINAL SPEC: visitCount parameter removed; always initialized to 0 (Deviation 2)
Guest::Guest(const char* name, int id)
    : Person(name, id), visitCount(0),
      tickets(new Ticket*[INITIAL_TICKETS_CAPACITY]),
      numTickets(0), ticketsCapacity(INITIAL_TICKETS_CAPACITY) {}

Guest::Guest(const Guest& other)
    : Person(other), visitCount(other.visitCount),
      tickets(new Ticket*[other.ticketsCapacity]),
      numTickets(other.numTickets), ticketsCapacity(other.ticketsCapacity) {
    for (int i = 0; i < numTickets; ++i)
        tickets[i] = other.tickets[i]->clone();  // polymorphic: preserves VIPTicket type
}

Guest& Guest::operator=(const Guest& other) {
    if (this == &other) return *this;
    Person::operator=(other);
    for (int i = 0; i < numTickets; ++i)
        delete tickets[i];
    delete[] tickets;
    visitCount       = other.visitCount;
    ticketsCapacity  = other.ticketsCapacity;
    numTickets       = other.numTickets;
    tickets          = new Ticket*[ticketsCapacity];
    for (int i = 0; i < numTickets; ++i)
        tickets[i] = other.tickets[i]->clone();
    return *this;
}

Guest::~Guest() {
    for (int i = 0; i < numTickets; ++i)
        delete tickets[i];
    delete[] tickets;
}

int         Guest::getVisitCount()  const { return visitCount;  }
int         Guest::getNumTickets()  const { return numTickets;  }

const Ticket* Guest::getTicket(int index) const {
    if (index < 0 || index >= numTickets)
        throw std::out_of_range("Ticket index out of range.");
    return tickets[index];
}

void Guest::setVisitCount(int v)    { visitCount = v;       }
void Guest::incrementVisitCount()   { ++visitCount;         }

void Guest::resizeTickets() {
    int newCap = ticketsCapacity * 2;
    Ticket** newArr = new Ticket*[newCap];
    for (int i = 0; i < numTickets; ++i)
        newArr[i] = tickets[i];
    delete[] tickets;
    tickets         = newArr;
    ticketsCapacity = newCap;
}

void Guest::addTicket(Ticket* t) {
    if (!t) throw std::invalid_argument("Ticket pointer cannot be null.");
    if (numTickets == ticketsCapacity) resizeTickets();
    tickets[numTickets++] = t;
    incrementVisitCount();
}

void Guest::printDetails() const {
    std::cout << "Guest: "    << getName()
              << " | ID: "    << getId()
              << " | Visits: " << visitCount
              << " | Tickets: " << numTickets << std::endl;
}

Guest* Guest::clone() const {
    return new Guest(*this);
}
