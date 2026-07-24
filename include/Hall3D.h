#ifndef HALL3D_H
#define HALL3D_H

#include "Hall.h"

class Hall3D : virtual public Hall {
private:
    int glassesCount;

public:
    // CHANGED: removed Movie parameter; DEVIATION FROM ORIGINAL SPEC (Deviation 1)
    Hall3D(int hallNumber, int glassesCount);
    Hall3D(const Hall3D& other);
    Hall3D& operator=(const Hall3D& other) = delete;
    ~Hall3D() override;

    int  getGlassesCount() const;
    void setGlassesCount(int c);

    // DEVIATION FROM ORIGINAL SPEC: overrides for type/capacity queries (Deviations 3, 8)
    const char* getHallType()       const override; // returns "3D"
    int         get3DGlassesCount() const override; // returns glassesCount

    void  printHall() const override;
    Hall* clone()     const override;
};

#endif
