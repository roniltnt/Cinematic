#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Guest.h"

class Customer : public Guest {
    // DEVIATION FROM ORIGINAL SPEC: clubPoints member removed entirely (Deviation 1)
    // DEVIATION FROM ORIGINAL SPEC: visitCount constructor parameter removed; auto-initializes to 0 (Deviation 2)
public:
    Customer(const char* name, int id);
    Customer(const Customer& other);
    Customer& operator=(const Customer& other);

    void printDetails() const override;
    Customer* clone() const override; // CHANGED: covariant clone for polymorphic Guest copy
};

#endif
