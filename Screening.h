#ifndef SCREENING_H
#define SCREENING_H

// DEVIATION FROM ORIGINAL SPEC: New class representing a scheduled Movie-in-Hall event
// (Deviation 2). Owns seat-availability tracking (Deviation 6).

#include "Movie.h"
#include "Hall.h"
#include "Date.h"
#include <iostream>

class Screening {
private:
    const Movie& movieRef; // non-owning reference — Cinema manages Movie lifetime
    const Hall&  hallRef;  // non-owning reference — Cinema manages Hall lifetime
    Date screeningDate;
    int  startHour;        // 0-23
    int  startMinute;      // 0-59
    bool is3DScreening;    // DEVIATION FROM ORIGINAL SPEC: per-screening 3D flag (Deviation 2)
    int  totalSeats;       // DEVIATION FROM ORIGINAL SPEC: may be capped by glassesCount (Deviation 3)
    int  soldTickets;      // DEVIATION FROM ORIGINAL SPEC: per-screening seat counter (Deviation 6)

public:
    Screening(const Movie& movie, const Hall& hall, const Date& date,
              int startHour, int startMinute, bool is3DScreening, int totalSeats);
    Screening(const Screening& other); // copy binds to same Movie/Hall (reference aliasing)
    Screening& operator=(const Screening& other) = delete; // reference members prevent assignment
    ~Screening();

    const Movie& getMovie()          const;
    const Hall&  getHall()           const;
    const Date&  getDate()           const;
    int          getStartHour()      const;
    int          getStartMinute()    const;
    bool         getIs3DScreening()  const;
    int          getTotalSeats()     const;
    int          getSoldTickets()    const;
    int          getAvailableSeats() const;

    int  getStartTimeMinutes() const; // total minutes from midnight
    int  getEndTimeMinutes()   const; // start + movie length

    // DEVIATION FROM ORIGINAL SPEC: time-overlap check used by Cinema::scheduleScreening (Deviation 2)
    bool overlapsWith(const Screening& other) const;

    // DEVIATION FROM ORIGINAL SPEC: decrements available seat count (Deviation 6)
    void sellSeat();

    // DEVIATION FROM ORIGINAL SPEC: detailed screening display for table printing (Deviation 7)
    void printScreening() const;
};

#endif
