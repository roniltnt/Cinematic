#ifndef HALL3DVIP_H
#define HALL3DVIP_H

#include "VIPHall.h"
#include "Hall3D.h"

class Hall3DVIP : public VIPHall, public Hall3D {
public:
    // CHANGED: removed Movie parameter; DEVIATION FROM ORIGINAL SPEC (Deviation 1)
    Hall3DVIP(int hallNumber, int waitersCount, int glassesCount);
    Hall3DVIP(const Hall3DVIP& other);
    Hall3DVIP& operator=(const Hall3DVIP& other) = delete;
    ~Hall3DVIP() override;

    // DEVIATION FROM ORIGINAL SPEC: must override here to resolve VIPHall/"3D" vs "VIP" ambiguity
    const char* getHallType() const override; // returns "3D+VIP"

    void  printHall() const override;
    Hall* clone()     const override;
};

#endif
