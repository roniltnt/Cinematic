#ifndef CINEMA_H
#define CINEMA_H

#include "Employee.h"
#include "Guest.h"
#include "Hall.h"
#include "Movie.h"
#include "Shift.h"
#include "Screening.h" // DEVIATION FROM ORIGINAL SPEC: new entity (Deviation 2)
#include "Date.h"

class Cinema {
public:
    static const int INITIAL_CAPACITY = 10;

private:
    Employee** employees;
    int numEmployees;
    int employeesCapacity;

    Guest** guests;
    int numGuests;
    int guestsCapacity;

    Hall** halls;
    int numHalls;
    int hallsCapacity;

    Movie** movies;
    int numMovies;
    int moviesCapacity;

    Shift** shifts;
    int numShifts;
    int shiftsCapacity;

    // DEVIATION FROM ORIGINAL SPEC: per-screening scheduling (Deviation 2)
    Screening** screenings;
    int numScreenings;
    int screeningsCapacity;

    void resizeEmployees();
    void resizeGuests();
    void resizeHalls();
    void resizeMovies();
    void resizeShifts();
    void resizeScreenings(); // DEVIATION FROM ORIGINAL SPEC

    // Used by operator= (copy-and-swap) so a throw during copying leaves *this untouched.
    void swap(Cinema& other) noexcept;

public:
    Cinema();
    Cinema(const Cinema& other);
    Cinema& operator=(const Cinema& other);
    ~Cinema();

    int getNumEmployees()  const;
    int getNumGuests()     const;
    int getNumHalls()      const;
    int getNumMovies()     const;
    int getNumShifts()     const;
    int getNumScreenings() const; // DEVIATION FROM ORIGINAL SPEC

    const Employee*  getEmployeeByIndex(int i)  const;
    Employee*        getEmployeeByIndex(int i);
    const Guest*     getGuestByIndex(int i)     const;
    Guest*           getGuestByIndex(int i);
    const Hall*      getHallByIndex(int i)      const;
    Hall*            getHallByIndex(int i);
    const Movie*     getMovieByIndex(int i)     const;
    Movie*           getMovieByIndex(int i);
    const Shift*     getShiftByIndex(int i)     const;
    const Screening* getScreeningByIndex(int i) const; // DEVIATION FROM ORIGINAL SPEC
    Screening*       getScreeningByIndex(int i);       // DEVIATION FROM ORIGINAL SPEC

    Guest*    findGuestById(int id);
    Employee* findEmployeeById(int id);

    Cinema& operator+=(Employee* e);
    Cinema& operator+=(Guest* g);
    Cinema& operator+=(Hall* h);
    Cinema& operator+=(Movie* m);
    Cinema& operator+=(Shift* s);
    Cinema& operator+=(Screening* sc); // DEVIATION FROM ORIGINAL SPEC

    Cinema& operator-=(int guestId);

    // DEVIATION FROM ORIGINAL SPEC: Screening scheduling with overlap + 3D validation (Deviation 2, 3)
    bool canSchedule3D(int movieIdx, int hallIdx) const;
    void scheduleScreening(int movieIdx, int hallIdx, const Date& date,
                           int startHour, int startMinute, bool is3DScreening);

    // DEVIATION FROM ORIGINAL SPEC: ticket type auto-determined from Screening/Hall — no bool params (Deviations 3, 5)
    // DEVIATION FROM ORIGINAL SPEC: VIP ticket only allowed when Hall is a VIP Hall (Deviation 3)
    void sellTicket(int screeningIdx, int guestIdx);

    void printAllGuests()     const;
    void printAllEmployees()  const;
    void printAllShifts()     const;
    // DEVIATION FROM ORIGINAL SPEC: ASCII table, 1-based index (Deviations 7, 8)
    void printAllHalls()      const;
    void printAllMovies()     const;
    void printAllScreenings() const; // DEVIATION FROM ORIGINAL SPEC (Deviation 7)
};

#endif
