#ifndef HALL3DVIP_H
#define HALL3DVIP_H

#include "VIPHall.h"
#include "Hall3D.h"

class Hall3DVIP : public VIPHall, public Hall3D {
public:
    Hall3DVIP(int hallNumber, const Movie& currentMovie, int waitersCount, int glassesCount); // CHANGED: const Movie&
    Hall3DVIP(const Hall3DVIP& other);
    Hall3DVIP& operator=(const Hall3DVIP& other) = delete;
    ~Hall3DVIP() override;

    void printHall() const override;
    Hall* clone() const override; // CHANGED: returns Hall* (not covariant) — MSVC rejects covariant overrides across a virtual-inheritance diamond
};

#endif
