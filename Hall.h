#ifndef HALL_H
#define HALL_H

#include "Movie.h"

class Hall {
public:
    static const int NUM_SEATS = 100;

private:
    int hallNumber;
    bool seats[NUM_SEATS];
    const Movie& currentMovie; // CHANGED: const ref — non-const ref breaks copy ctor (can't bind Movie& from const Hall&)

public:
    Hall(int hallNumber, const Movie& currentMovie); // CHANGED: parameter is now const Movie&
    Hall(const Hall& other);
    Hall& operator=(const Hall& other) = delete;
    virtual ~Hall();

    int getHallNumber() const;
    const Movie& getCurrentMovie() const;
    bool isSeatTaken(int seatIndex) const;
    int countTakenSeats() const;

    void setHallNumber(int num);

    void takeSeat(int seatIndex);
    void freeSeat(int seatIndex);

    bool operator!() const;

    virtual void printHall() const;
    virtual Hall* clone() const; // CHANGED: virtual clone for polymorphic copy in Cinema
};

#endif
