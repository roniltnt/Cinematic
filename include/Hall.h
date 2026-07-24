#ifndef HALL_H
#define HALL_H

#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: Hall no longer stores a Movie reference or a seats array.
// It is a pure physical description. Seat tracking moves to Screening (Deviations 1, 6).
class Hall {
public:
    static const int NUM_SEATS = 100;

private:
    int hallNumber;

public:
    // CHANGED: constructor no longer takes a Movie reference
    Hall(int hallNumber);
    Hall(const Hall& other);
    Hall& operator=(const Hall& other) = delete;
    virtual ~Hall();

    int  getHallNumber() const;
    void setHallNumber(int num);

    // DEVIATION FROM ORIGINAL SPEC: virtual type/capacity queries used by Screening
    // and Cinema to implement 3D capacity limiting (Deviation 3) and table printing (Deviation 8).
    virtual const char* getHallType()        const; // returns "Regular" in base
    virtual int         get3DGlassesCount()  const; // returns 0 in base (not 3D capable)
    virtual int         getWaitersCount()    const; // returns 0 in base (not VIP)

    virtual void  printHall() const;
    virtual Hall* clone()     const;
};

#endif
