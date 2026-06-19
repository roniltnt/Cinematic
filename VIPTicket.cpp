#include "VIPTicket.h"

const double VIPTicket::VIP_SURCHARGE = 25.0;
const double VIPTicket::MEAL_PRICE    = 15.0;

VIPTicket::VIPTicket(const Movie& movie, bool is3D, bool includesMeal)
    : Ticket(movie, is3D), includesMeal(includesMeal) {}

VIPTicket::VIPTicket(const VIPTicket& other)
    : Ticket(other), includesMeal(other.includesMeal) {}

VIPTicket::~VIPTicket() {}

bool VIPTicket::getIncludesMeal() const  { return includesMeal; }
void VIPTicket::setIncludesMeal(bool m)  { includesMeal = m;    }

double VIPTicket::calcFinalPrice() const {
    return Ticket::calcFinalPrice() + VIP_SURCHARGE + (includesMeal ? MEAL_PRICE : 0.0);
}

VIPTicket* VIPTicket::clone() const {
    return new VIPTicket(*this);
}
