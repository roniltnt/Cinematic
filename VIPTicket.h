#ifndef VIPTICKET_H
#define VIPTICKET_H

#include "Ticket.h"

// DEVIATION FROM ORIGINAL SPEC: VIP_SURCHARGE, MEAL_PRICE, and calcFinalPrice()
// removed (Deviation 4). Meal flag is kept as a service feature.
class VIPTicket : public Ticket {
private:
    bool includesMeal;

public:
    // CHANGED: constructor now takes Screening reference (Deviation 5)
    VIPTicket(const Screening& screening, bool is3D, bool includesMeal);
    VIPTicket(const VIPTicket& other);
    VIPTicket& operator=(const VIPTicket& other) = delete;
    ~VIPTicket() override;

    bool getIncludesMeal() const;
    void setIncludesMeal(bool m);

    void       printTicket() const override; // CHANGED: replaces calcFinalPrice()
    VIPTicket* clone()       const override;
};

#endif
