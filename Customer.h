#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Guest.h"

class Customer : public Guest {
private:
    int clubPoints;

public:
    Customer(const char* name, int id, int clubPoints = 0, int visitCount = 0);
    Customer(const Customer& other);
    Customer& operator=(const Customer& other);

    int getClubPoints() const;
    void setClubPoints(int p);
    void addClubPoints(int p);

    void printDetails() const override;
    Customer* clone() const override; // CHANGED: covariant clone for polymorphic Guest copy
};

#endif
