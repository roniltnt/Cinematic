#include "Ticket.h"

const double Ticket::BASE_PRICE        = 30.0;
const double Ticket::THREE_D_SURCHARGE = 10.0;

// movieRef is a const reference — binds to the Movie managed by Cinema
Ticket::Ticket(const Movie& movie, bool is3D)
    : movieRef(movie), is3D(is3D) {}

// Copy binds to the same Movie; Ticket does not own its movie
Ticket::Ticket(const Ticket& other)
    : movieRef(other.movieRef), is3D(other.is3D) {}

Ticket::~Ticket() {}  // virtual; no owned dynamic memory

const Movie& Ticket::getMovie() const { return movieRef; }
bool         Ticket::getIs3D()  const { return is3D;     }

void Ticket::setIs3D(bool flag) { is3D = flag; }

double Ticket::calcFinalPrice() const {
    return BASE_PRICE + (is3D ? THREE_D_SURCHARGE : 0.0);
}

bool Ticket::operator>(const Ticket& other) const {
    return calcFinalPrice() > other.calcFinalPrice();
}

Ticket* Ticket::clone() const {
    return new Ticket(*this);
}
