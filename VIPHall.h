#ifndef VIPHALL_H
#define VIPHALL_H

#include "Hall.h"

class VIPHall : virtual public Hall {
private:
    int waitersCount;

public:
    VIPHall(int hallNumber, const Movie& currentMovie, int waitersCount); // CHANGED: const Movie&
    VIPHall(const VIPHall& other);
    VIPHall& operator=(const VIPHall& other) = delete;
    ~VIPHall() override;

    int getWaitersCount() const;
    void setWaitersCount(int c);

    void printHall() const override;
    Hall* clone() const override; // CHANGED: returns Hall* (not covariant) — MSVC rejects covariant overrides across a virtual-inheritance diamond
};

#endif
