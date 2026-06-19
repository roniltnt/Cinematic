#ifndef HALL3D_H
#define HALL3D_H

#include "Hall.h"

class Hall3D : virtual public Hall {
private:
    int glassesCount;

public:
    Hall3D(int hallNumber, const Movie& currentMovie, int glassesCount); // CHANGED: const Movie&
    Hall3D(const Hall3D& other);
    Hall3D& operator=(const Hall3D& other) = delete;
    ~Hall3D() override;

    int getGlassesCount() const;
    void setGlassesCount(int c);

    void printHall() const override;
    Hall* clone() const override; // CHANGED: returns Hall* (not covariant) — MSVC rejects covariant overrides across a virtual-inheritance diamond
};

#endif
