#ifndef TICKET_H
#define TICKET_H

#include "Movie.h"

class Ticket {
private:
    const Movie& movieRef;
    bool is3D;

    static const double BASE_PRICE;
    static const double THREE_D_SURCHARGE;

public:
    Ticket(const Movie& movie, bool is3D);
    Ticket(const Ticket& other);
    Ticket& operator=(const Ticket& other) = delete;
    virtual ~Ticket();

    const Movie& getMovie() const;
    bool getIs3D() const;

    void setIs3D(bool flag);

    virtual double calcFinalPrice() const;

    bool operator>(const Ticket& other) const;
    virtual Ticket* clone() const; // CHANGED: virtual clone needed for polymorphic copy in Guest
};

#endif
