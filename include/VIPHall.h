#ifndef VIPHALL_H
#define VIPHALL_H

#include "Hall.h"

class VIPHall : virtual public Hall {
private:
    int waitersCount;

public:
    // CHANGED: removed Movie parameter; DEVIATION FROM ORIGINAL SPEC (Deviation 1)
    VIPHall(int hallNumber, int waitersCount);
    VIPHall(const VIPHall& other);
    VIPHall& operator=(const VIPHall& other) = delete;
    ~VIPHall() override;

    // DEVIATION FROM ORIGINAL SPEC: overrides Hall's virtual queries (Deviations 3, 8)
    const char* getHallType()    const override; // returns "VIP"
    int         getWaitersCount() const override; // CHANGED: satisfies Hall's virtual; was non-virtual getter

    void setWaitersCount(int c);

    void  printHall() const override;
    Hall* clone()     const override;
};

#endif
