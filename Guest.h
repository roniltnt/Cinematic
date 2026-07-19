#ifndef GUEST_H
#define GUEST_H

#include "Person.h"

class Ticket;

class Guest : public Person {
public:
    static const int INITIAL_TICKETS_CAPACITY = 4;

private:
    int visitCount;
    Ticket** tickets;
    int numTickets;
    int ticketsCapacity;

    void resizeTickets();

public:
    // DEVIATION FROM ORIGINAL SPEC: visitCount parameter removed; always auto-initialized to 0 (Deviation 2)
    Guest(const char* name, int id);
    Guest(const Guest& other);
    Guest& operator=(const Guest& other);
    virtual ~Guest();

    int getVisitCount() const;
    int getNumTickets() const;
    const Ticket* getTicket(int index) const;

    void setVisitCount(int v);
    void incrementVisitCount();

    void addTicket(Ticket* t);

    void printDetails() const override;
    virtual Guest* clone() const; // CHANGED: virtual clone needed for polymorphic copy in Cinema::operator=
};

#endif
