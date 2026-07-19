#include "Cinema.h"
#include "VIPTicket.h" // needed only in sellTicket() — kept out of Cinema.h to limit coupling
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <stdexcept>

// ============================================================
//  ASCII-table helpers (file-scope only)
// ============================================================
static void printChars(char c, int n) {
    for (int i = 0; i < n; ++i) std::cout << c;
}

// Print one horizontal rule given an array of column widths.
// Each cell occupies (width + 2) chars (1 space padding each side) plus '+' separators.
static void tableRule(const int* w, int cols) {
    std::cout << '+';
    for (int i = 0; i < cols; ++i) { printChars('-', w[i] + 2); std::cout << '+'; }
    std::cout << '\n';
}

// Print a cell value left-aligned, truncated if longer than width.
static void tableCell(const char* str, int width) {
    int len = (int)strlen(str);
    if (len <= width) {
        std::cout << ' ' << std::left << std::setw(width) << str << ' ';
    } else {
        std::cout << ' ';
        for (int i = 0; i < width - 3; ++i) std::cout << str[i];
        std::cout << "... ";
    }
}

static void tableCellInt(int val, int width) {
    std::cout << ' ' << std::right << std::setw(width) << val << ' ';
}

// ============================================================
//  Constructor
// ============================================================
Cinema::Cinema()
    : employees(new Employee*[INITIAL_CAPACITY]), numEmployees(0), employeesCapacity(INITIAL_CAPACITY),
      guests   (new Guest*   [INITIAL_CAPACITY]), numGuests(0),    guestsCapacity(INITIAL_CAPACITY),
      halls    (new Hall*    [INITIAL_CAPACITY]), numHalls(0),     hallsCapacity(INITIAL_CAPACITY),
      movies   (new Movie*   [INITIAL_CAPACITY]), numMovies(0),    moviesCapacity(INITIAL_CAPACITY),
      shifts   (new Shift*   [INITIAL_CAPACITY]), numShifts(0),    shiftsCapacity(INITIAL_CAPACITY),
      screenings(new Screening*[INITIAL_CAPACITY]), numScreenings(0), screeningsCapacity(INITIAL_CAPACITY) {}

// ============================================================
//  Copy Constructor
//  Design note: Screening::movieRef and Screening::hallRef are reference members —
//  copied Screenings (and Tickets inside copied Guests) still alias the ORIGINAL
//  Cinema's Movie/Hall/Screening objects. This is a known constraint of reference-
//  member design and does not affect the 14-action demo (Cinema is never copied).
// ============================================================
Cinema::Cinema(const Cinema& other)
    : employees(new Employee*[other.employeesCapacity]),
      numEmployees(other.numEmployees), employeesCapacity(other.employeesCapacity),
      guests   (new Guest*   [other.guestsCapacity]),
      numGuests(other.numGuests),       guestsCapacity(other.guestsCapacity),
      halls    (new Hall*    [other.hallsCapacity]),
      numHalls(other.numHalls),         hallsCapacity(other.hallsCapacity),
      movies   (new Movie*   [other.moviesCapacity]),
      numMovies(other.numMovies),       moviesCapacity(other.moviesCapacity),
      shifts   (new Shift*   [other.shiftsCapacity]),
      numShifts(other.numShifts),       shiftsCapacity(other.shiftsCapacity),
      screenings(new Screening*[other.screeningsCapacity]),
      numScreenings(other.numScreenings), screeningsCapacity(other.screeningsCapacity) {
    for (int i = 0; i < numEmployees;  ++i) employees[i]  = new Employee(*other.employees[i]);
    for (int i = 0; i < numGuests;     ++i) guests[i]     = other.guests[i]->clone();
    for (int i = 0; i < numMovies;     ++i) movies[i]     = new Movie(*other.movies[i]);
    for (int i = 0; i < numHalls;      ++i) halls[i]      = other.halls[i]->clone();
    for (int i = 0; i < numScreenings; ++i) screenings[i] = new Screening(*other.screenings[i]);
    for (int i = 0; i < numShifts;     ++i) shifts[i]     = new Shift(*other.shifts[i]);
}

// ============================================================
//  Assignment Operator
// ============================================================
Cinema& Cinema::operator=(const Cinema& other) {
    if (this == &other) return *this;
    for (int i = 0; i < numShifts;     ++i) delete shifts[i];
    for (int i = 0; i < numScreenings; ++i) delete screenings[i];
    for (int i = 0; i < numGuests;     ++i) delete guests[i];
    for (int i = 0; i < numEmployees;  ++i) delete employees[i];
    for (int i = 0; i < numHalls;      ++i) delete halls[i];
    for (int i = 0; i < numMovies;     ++i) delete movies[i];
    delete[] shifts;  delete[] screenings; delete[] guests;
    delete[] employees; delete[] halls;   delete[] movies;

    numEmployees  = other.numEmployees;  employeesCapacity  = other.employeesCapacity;
    numGuests     = other.numGuests;     guestsCapacity     = other.guestsCapacity;
    numHalls      = other.numHalls;      hallsCapacity      = other.hallsCapacity;
    numMovies     = other.numMovies;     moviesCapacity     = other.moviesCapacity;
    numShifts     = other.numShifts;     shiftsCapacity     = other.shiftsCapacity;
    numScreenings = other.numScreenings; screeningsCapacity = other.screeningsCapacity;

    employees  = new Employee* [employeesCapacity];
    guests     = new Guest*    [guestsCapacity];
    halls      = new Hall*     [hallsCapacity];
    movies     = new Movie*    [moviesCapacity];
    shifts     = new Shift*    [shiftsCapacity];
    screenings = new Screening*[screeningsCapacity];

    for (int i = 0; i < numEmployees;  ++i) employees[i]  = new Employee(*other.employees[i]);
    for (int i = 0; i < numGuests;     ++i) guests[i]     = other.guests[i]->clone();
    for (int i = 0; i < numMovies;     ++i) movies[i]     = new Movie(*other.movies[i]);
    for (int i = 0; i < numHalls;      ++i) halls[i]      = other.halls[i]->clone();
    for (int i = 0; i < numScreenings; ++i) screenings[i] = new Screening(*other.screenings[i]);
    for (int i = 0; i < numShifts;     ++i) shifts[i]     = new Shift(*other.shifts[i]);
    return *this;
}

// ============================================================
//  Destructor — dependency order:
//    shifts -> employees  (Shift holds const Employee&)
//    guests  -> screenings (Ticket holds const Screening&)
//    screenings -> movies, halls  (Screening holds const Movie& and const Hall&)
//    halls, movies last
// ============================================================
Cinema::~Cinema() {
    for (int i = 0; i < numShifts;     ++i) delete shifts[i];     delete[] shifts;
    for (int i = 0; i < numEmployees;  ++i) delete employees[i];  delete[] employees;
    for (int i = 0; i < numGuests;     ++i) delete guests[i];     delete[] guests;
    for (int i = 0; i < numScreenings; ++i) delete screenings[i]; delete[] screenings;
    for (int i = 0; i < numHalls;      ++i) delete halls[i];      delete[] halls;
    for (int i = 0; i < numMovies;     ++i) delete movies[i];     delete[] movies;
}

// ============================================================
//  Resize helpers
// ============================================================
void Cinema::resizeEmployees() {
    int c = employeesCapacity * 2;
    Employee** t = new Employee*[c];
    for (int i = 0; i < numEmployees; ++i) t[i] = employees[i];
    delete[] employees; employees = t; employeesCapacity = c;
}
void Cinema::resizeGuests() {
    int c = guestsCapacity * 2;
    Guest** t = new Guest*[c];
    for (int i = 0; i < numGuests; ++i) t[i] = guests[i];
    delete[] guests; guests = t; guestsCapacity = c;
}
void Cinema::resizeHalls() {
    int c = hallsCapacity * 2;
    Hall** t = new Hall*[c];
    for (int i = 0; i < numHalls; ++i) t[i] = halls[i];
    delete[] halls; halls = t; hallsCapacity = c;
}
void Cinema::resizeMovies() {
    int c = moviesCapacity * 2;
    Movie** t = new Movie*[c];
    for (int i = 0; i < numMovies; ++i) t[i] = movies[i];
    delete[] movies; movies = t; moviesCapacity = c;
}
void Cinema::resizeShifts() {
    int c = shiftsCapacity * 2;
    Shift** t = new Shift*[c];
    for (int i = 0; i < numShifts; ++i) t[i] = shifts[i];
    delete[] shifts; shifts = t; shiftsCapacity = c;
}
// DEVIATION FROM ORIGINAL SPEC: resize for Screenings array (Deviation 2)
void Cinema::resizeScreenings() {
    int c = screeningsCapacity * 2;
    Screening** t = new Screening*[c];
    for (int i = 0; i < numScreenings; ++i) t[i] = screenings[i];
    delete[] screenings; screenings = t; screeningsCapacity = c;
}

// ============================================================
//  Getters
// ============================================================
int Cinema::getNumEmployees()  const { return numEmployees;  }
int Cinema::getNumGuests()     const { return numGuests;     }
int Cinema::getNumHalls()      const { return numHalls;      }
int Cinema::getNumMovies()     const { return numMovies;     }
int Cinema::getNumShifts()     const { return numShifts;     }
int Cinema::getNumScreenings() const { return numScreenings; } // DEVIATION FROM ORIGINAL SPEC

const Employee*  Cinema::getEmployeeByIndex(int i)  const { if(i<0||i>=numEmployees)  throw std::out_of_range("Employee index OOB.");  return employees[i];  }
Employee*        Cinema::getEmployeeByIndex(int i)        { if(i<0||i>=numEmployees)  throw std::out_of_range("Employee index OOB.");  return employees[i];  }
const Guest*     Cinema::getGuestByIndex(int i)     const { if(i<0||i>=numGuests)     throw std::out_of_range("Guest index OOB.");     return guests[i];     }
Guest*           Cinema::getGuestByIndex(int i)           { if(i<0||i>=numGuests)     throw std::out_of_range("Guest index OOB.");     return guests[i];     }
const Hall*      Cinema::getHallByIndex(int i)      const { if(i<0||i>=numHalls)      throw std::out_of_range("Hall index OOB.");      return halls[i];      }
Hall*            Cinema::getHallByIndex(int i)            { if(i<0||i>=numHalls)      throw std::out_of_range("Hall index OOB.");      return halls[i];      }
const Movie*     Cinema::getMovieByIndex(int i)     const { if(i<0||i>=numMovies)     throw std::out_of_range("Movie index OOB.");     return movies[i];     }
Movie*           Cinema::getMovieByIndex(int i)           { if(i<0||i>=numMovies)     throw std::out_of_range("Movie index OOB.");     return movies[i];     }
const Shift*     Cinema::getShiftByIndex(int i)     const { if(i<0||i>=numShifts)     throw std::out_of_range("Shift index OOB.");     return shifts[i];     }
// DEVIATION FROM ORIGINAL SPEC
const Screening* Cinema::getScreeningByIndex(int i) const { if(i<0||i>=numScreenings) throw std::out_of_range("Screening index OOB."); return screenings[i]; }
Screening*       Cinema::getScreeningByIndex(int i)       { if(i<0||i>=numScreenings) throw std::out_of_range("Screening index OOB."); return screenings[i]; }

Guest*    Cinema::findGuestById(int id)    { for(int i=0;i<numGuests;++i)    if(guests[i]->getId()==id)    return guests[i];    return nullptr; }
Employee* Cinema::findEmployeeById(int id) { for(int i=0;i<numEmployees;++i) if(employees[i]->getId()==id) return employees[i]; return nullptr; }

// ============================================================
//  operator+=
// ============================================================
Cinema& Cinema::operator+=(Employee* e)  { if(!e) throw std::invalid_argument("Null Employee.");  if(numEmployees==employeesCapacity)  resizeEmployees();  employees[numEmployees++]=e;   return *this; }
Cinema& Cinema::operator+=(Guest* g)     { if(!g) throw std::invalid_argument("Null Guest.");     if(numGuests==guestsCapacity)        resizeGuests();     guests[numGuests++]=g;         return *this; }
Cinema& Cinema::operator+=(Hall* h)      { if(!h) throw std::invalid_argument("Null Hall.");      if(numHalls==hallsCapacity)          resizeHalls();      halls[numHalls++]=h;           return *this; }
Cinema& Cinema::operator+=(Movie* m)     { if(!m) throw std::invalid_argument("Null Movie.");     if(numMovies==moviesCapacity)        resizeMovies();     movies[numMovies++]=m;         return *this; }
Cinema& Cinema::operator+=(Shift* s)     { if(!s) throw std::invalid_argument("Null Shift.");     if(numShifts==shiftsCapacity)        resizeShifts();     shifts[numShifts++]=s;         return *this; }
// DEVIATION FROM ORIGINAL SPEC
Cinema& Cinema::operator+=(Screening* sc){ if(!sc) throw std::invalid_argument("Null Screening."); if(numScreenings==screeningsCapacity) resizeScreenings(); screenings[numScreenings++]=sc; return *this; }

// ============================================================
//  operator-=  (remove guest by ID)
// ============================================================
Cinema& Cinema::operator-=(int guestId) {
    int idx = -1;
    for (int i = 0; i < numGuests; ++i) {
        if (guests[i]->getId() == guestId) { idx = i; break; }
    }
    if (idx == -1) throw std::invalid_argument("No guest found with that ID.");
    delete guests[idx];
    for (int i = idx; i < numGuests - 1; ++i) guests[i] = guests[i + 1];
    guests[--numGuests] = nullptr;
    return *this;
}

// ============================================================
//  DEVIATION FROM ORIGINAL SPEC: Screening scheduling with validations (Deviations 2, 3)
// ============================================================
bool Cinema::canSchedule3D(int movieIdx, int hallIdx) const {
    return getMovieByIndex(movieIdx)->getIs3D() &&
           getHallByIndex(hallIdx)->get3DGlassesCount() > 0;
}

void Cinema::scheduleScreening(int movieIdx, int hallIdx, const Date& date,
                                int startHour, int startMinute, bool is3DScreening) {
    Movie* movie = getMovieByIndex(movieIdx);
    Hall*  hall  = getHallByIndex(hallIdx);

    // DEVIATION FROM ORIGINAL SPEC: cap capacity by 3D glasses count (Deviation 3)
    int totalSeats = Hall::NUM_SEATS;
    if (is3DScreening) {
        int glasses = hall->get3DGlassesCount();
        if (glasses > 0 && glasses < totalSeats) totalSeats = glasses;
    }

    Screening* sc = new Screening(*movie, *hall, date, startHour, startMinute,
                                  is3DScreening, totalSeats);

    // DEVIATION FROM ORIGINAL SPEC: overlap validation (Deviation 2)
    for (int i = 0; i < numScreenings; ++i) {
        if (screenings[i]->overlapsWith(*sc)) {
            delete sc;
            throw std::runtime_error(
                "Scheduling conflict: the chosen time overlaps with an existing screening in the same hall.");
        }
    }

    *this += sc;
}

// ============================================================
//  DEVIATION FROM ORIGINAL SPEC: sell ticket for a specific Screening (Deviations 5, 6)
// ============================================================
void Cinema::sellTicket(int screeningIdx, int guestIdx, bool is3D, bool isVIP, bool includesMeal) {
    Screening* sc = getScreeningByIndex(screeningIdx);
    if (sc->getAvailableSeats() == 0)
        throw std::runtime_error("No seats available for this screening.");

    Ticket* t = isVIP
        ? static_cast<Ticket*>(new VIPTicket(*sc, is3D, includesMeal))
        : new Ticket(*sc, is3D);

    try {
        sc->sellSeat();                          // DEVIATION FROM ORIGINAL SPEC: decrement seat count (Deviation 6)
        getGuestByIndex(guestIdx)->addTicket(t); // transfers ownership to Guest
    } catch (...) {
        delete t;
        throw;
    }
}

// ============================================================
//  Print helpers — 1-based index for all list output (Deviation 8)
// ============================================================
void Cinema::printAllEmployees() const {
    if (numEmployees == 0) { std::cout << "  (no employees registered)\n"; return; }
    for (int i = 0; i < numEmployees; ++i) {
        std::cout << "[" << (i + 1) << "] "; // DEVIATION FROM ORIGINAL SPEC: 1-based (Deviation 8)
        employees[i]->printDetails();
    }
}

void Cinema::printAllGuests() const {
    if (numGuests == 0) { std::cout << "  (no guests registered)\n"; return; }
    for (int i = 0; i < numGuests; ++i) {
        std::cout << "[" << (i + 1) << "] "; // DEVIATION FROM ORIGINAL SPEC: 1-based (Deviation 8)
        guests[i]->printDetails();            // virtual dispatch: Customer or Reviewer
    }
}

void Cinema::printAllShifts() const {
    if (numShifts == 0) { std::cout << "  (no shifts scheduled)\n"; return; }
    for (int i = 0; i < numShifts; ++i) {
        std::cout << "[" << (i + 1) << "] "; // DEVIATION FROM ORIGINAL SPEC: 1-based (Deviation 8)
        shifts[i]->printShift();
    }
}

// ============================================================
//  DEVIATION FROM ORIGINAL SPEC: ASCII table, 1-based index (Deviations 7, 8)
// ============================================================
void Cinema::printAllMovies() const {
    if (numMovies == 0) { std::cout << "  (no movies added)\n"; return; }

    // column widths: index, title, duration, 3D
    const int W[] = {3, 26, 10, 5};
    const int COLS = 4;
    const char* HEADERS[] = {"#", "Title", "Duration", "3D"};

    tableRule(W, COLS);
    std::cout << '|'; for (int c = 0; c < COLS; ++c) { tableCell(HEADERS[c], W[c]); std::cout << '|'; } std::cout << '\n';
    tableRule(W, COLS);

    for (int i = 0; i < numMovies; ++i) {
        char idx[8], dur[16];
        snprintf(idx, sizeof(idx), "%d", i + 1); // DEVIATION FROM ORIGINAL SPEC: 1-based (Deviation 8)
        snprintf(dur, sizeof(dur), "%d min", movies[i]->getLengthMinutes());

        std::cout << '|';
        tableCell(idx, W[0]);                        std::cout << '|';
        tableCell(movies[i]->getTitle(), W[1]);      std::cout << '|';
        tableCell(dur, W[2]);                        std::cout << '|';
        tableCell(movies[i]->getIs3D() ? "Yes" : "No", W[3]); std::cout << "|\n";
    }
    tableRule(W, COLS);
}

void Cinema::printAllHalls() const {
    if (numHalls == 0) { std::cout << "  (no halls added)\n"; return; }

    // column widths: index, hall#, type, waiters, glasses
    const int W[] = {3, 7, 8, 9, 9};
    const int COLS = 5;
    const char* HEADERS[] = {"#", "Hall #", "Type", "Waiters", "Glasses"};

    tableRule(W, COLS);
    std::cout << '|'; for (int c = 0; c < COLS; ++c) { tableCell(HEADERS[c], W[c]); std::cout << '|'; } std::cout << '\n';
    tableRule(W, COLS);

    for (int i = 0; i < numHalls; ++i) {
        char idx[8], hallNum[8], waiters[8], glasses[8];
        snprintf(idx,     sizeof(idx),     "%d", i + 1); // DEVIATION FROM ORIGINAL SPEC: 1-based (Deviation 8)
        snprintf(hallNum, sizeof(hallNum), "%d", halls[i]->getHallNumber());
        int w = halls[i]->getWaitersCount();
        int g = halls[i]->get3DGlassesCount();
        if (w > 0) snprintf(waiters, sizeof(waiters), "%d", w); else snprintf(waiters, sizeof(waiters), "-");
        if (g > 0) snprintf(glasses, sizeof(glasses), "%d", g); else snprintf(glasses, sizeof(glasses), "-");

        std::cout << '|';
        tableCell(idx,                       W[0]); std::cout << '|';
        tableCell(hallNum,                   W[1]); std::cout << '|';
        tableCell(halls[i]->getHallType(),   W[2]); std::cout << '|';
        tableCell(waiters,                   W[3]); std::cout << '|';
        tableCell(glasses,                   W[4]); std::cout << "|\n";
    }
    tableRule(W, COLS);
}

// DEVIATION FROM ORIGINAL SPEC: new print for Screenings (Deviation 7, 8)
void Cinema::printAllScreenings() const {
    if (numScreenings == 0) { std::cout << "  (no screenings scheduled)\n"; return; }

    // column widths: index, movie, hall#, date, start, 3D, seats (sold/total), free
    const int W[] = {3, 22, 7, 12, 6, 5, 10, 6};
    const int COLS = 8;
    const char* HEADERS[] = {"#", "Movie", "Hall #", "Date", "Start", "3D", "Sold/Tot", "Free"};

    tableRule(W, COLS);
    std::cout << '|'; for (int c = 0; c < COLS; ++c) { tableCell(HEADERS[c], W[c]); std::cout << '|'; } std::cout << '\n';
    tableRule(W, COLS);

    for (int i = 0; i < numScreenings; ++i) {
        const Screening* sc = screenings[i];
        char idx[8], hallNum[8], dateBuf[16], timeBuf[8], seatsBuf[16], freeBuf[8];

        snprintf(idx,      sizeof(idx),      "%d", i + 1); // DEVIATION FROM ORIGINAL SPEC: 1-based (Deviation 8)
        snprintf(hallNum,  sizeof(hallNum),  "%d", sc->getHall().getHallNumber());
        snprintf(dateBuf,  sizeof(dateBuf),  "%d/%d/%d",
                 sc->getDate().getDay(), sc->getDate().getMonth(), sc->getDate().getYear());
        snprintf(timeBuf,  sizeof(timeBuf),  "%02d:%02d", sc->getStartHour(), sc->getStartMinute());
        snprintf(seatsBuf, sizeof(seatsBuf), "%d/%d", sc->getSoldTickets(), sc->getTotalSeats());
        snprintf(freeBuf,  sizeof(freeBuf),  "%d", sc->getAvailableSeats());

        std::cout << '|';
        tableCell(idx,                                    W[0]); std::cout << '|';
        tableCell(sc->getMovie().getTitle(),              W[1]); std::cout << '|';
        tableCell(hallNum,                                W[2]); std::cout << '|';
        tableCell(dateBuf,                                W[3]); std::cout << '|';
        tableCell(timeBuf,                                W[4]); std::cout << '|';
        tableCell(sc->getIs3DScreening() ? "Yes" : "No", W[5]); std::cout << '|';
        tableCell(seatsBuf,                               W[6]); std::cout << '|';
        tableCell(freeBuf,                                W[7]); std::cout << "|\n";
    }
    tableRule(W, COLS);
}
