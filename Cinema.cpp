#include "Cinema.h"
#include <iostream>
#include <stdexcept>

// ============================================================
//  Constructor
// ============================================================
Cinema::Cinema()
    : employees(new Employee*[INITIAL_CAPACITY]), numEmployees(0), employeesCapacity(INITIAL_CAPACITY),
      guests   (new Guest*   [INITIAL_CAPACITY]), numGuests(0),    guestsCapacity(INITIAL_CAPACITY),
      halls    (new Hall*    [INITIAL_CAPACITY]), numHalls(0),     hallsCapacity(INITIAL_CAPACITY),
      movies   (new Movie*   [INITIAL_CAPACITY]), numMovies(0),    moviesCapacity(INITIAL_CAPACITY),
      shifts   (new Shift*   [INITIAL_CAPACITY]), numShifts(0),    shiftsCapacity(INITIAL_CAPACITY) {}

// ============================================================
//  Copy Constructor
//  Design note: Hall::currentMovie and Shift::workerRef are
//  reference members that cannot be reseated. Cloned halls
//  therefore alias the *original* Cinema's Movie objects, and
//  cloned shifts alias the original Cinema's Employee objects.
//  This is a known constraint of the reference-member design.
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
      numShifts(other.numShifts),       shiftsCapacity(other.shiftsCapacity) {
    for (int i = 0; i < numEmployees; ++i)
        employees[i] = new Employee(*other.employees[i]);
    for (int i = 0; i < numGuests; ++i)
        guests[i] = other.guests[i]->clone();      // preserves Customer / Reviewer type
    for (int i = 0; i < numMovies; ++i)
        movies[i] = new Movie(*other.movies[i]);
    for (int i = 0; i < numHalls; ++i)
        halls[i] = other.halls[i]->clone();        // preserves Hall3D / VIPHall / Hall3DVIP type
    for (int i = 0; i < numShifts; ++i)
        shifts[i] = new Shift(*other.shifts[i]);
}

// ============================================================
//  Assignment Operator
// ============================================================
Cinema& Cinema::operator=(const Cinema& other) {
    if (this == &other) return *this;

    // Release in dependency order before overwriting
    for (int i = 0; i < numShifts;    ++i) delete shifts[i];
    for (int i = 0; i < numHalls;     ++i) delete halls[i];
    for (int i = 0; i < numGuests;    ++i) delete guests[i];
    for (int i = 0; i < numEmployees; ++i) delete employees[i];
    for (int i = 0; i < numMovies;    ++i) delete movies[i];
    delete[] shifts;
    delete[] halls;
    delete[] guests;
    delete[] employees;
    delete[] movies;

    numEmployees = other.numEmployees; employeesCapacity = other.employeesCapacity;
    numGuests    = other.numGuests;    guestsCapacity    = other.guestsCapacity;
    numHalls     = other.numHalls;     hallsCapacity     = other.hallsCapacity;
    numMovies    = other.numMovies;    moviesCapacity    = other.moviesCapacity;
    numShifts    = other.numShifts;    shiftsCapacity    = other.shiftsCapacity;

    employees = new Employee*[employeesCapacity];
    guests    = new Guest*   [guestsCapacity];
    halls     = new Hall*    [hallsCapacity];
    movies    = new Movie*   [moviesCapacity];
    shifts    = new Shift*   [shiftsCapacity];

    for (int i = 0; i < numEmployees; ++i) employees[i] = new Employee(*other.employees[i]);
    for (int i = 0; i < numGuests;    ++i) guests[i]    = other.guests[i]->clone();
    for (int i = 0; i < numMovies;    ++i) movies[i]    = new Movie(*other.movies[i]);
    for (int i = 0; i < numHalls;     ++i) halls[i]     = other.halls[i]->clone();
    for (int i = 0; i < numShifts;    ++i) shifts[i]    = new Shift(*other.shifts[i]);

    return *this;
}

// ============================================================
//  Destructor — delete in dependency order:
//    shifts  -> employees  (Shift holds const Employee&)
//    guests  -> movies     (Ticket holds const Movie&)
//    halls   -> movies     (Hall   holds const Movie&)
//    movies  last
// ============================================================
Cinema::~Cinema() {
    for (int i = 0; i < numShifts;    ++i) delete shifts[i];
    delete[] shifts;
    for (int i = 0; i < numEmployees; ++i) delete employees[i];
    delete[] employees;
    for (int i = 0; i < numGuests;    ++i) delete guests[i];
    delete[] guests;
    for (int i = 0; i < numHalls;     ++i) delete halls[i];
    delete[] halls;
    for (int i = 0; i < numMovies;    ++i) delete movies[i];
    delete[] movies;
}

// ============================================================
//  Resize helpers — double capacity and move existing pointers
// ============================================================
void Cinema::resizeEmployees() {
    int newCap = employeesCapacity * 2;
    Employee** tmp = new Employee*[newCap];
    for (int i = 0; i < numEmployees; ++i) tmp[i] = employees[i];
    delete[] employees;
    employees = tmp;
    employeesCapacity = newCap;
}

void Cinema::resizeGuests() {
    int newCap = guestsCapacity * 2;
    Guest** tmp = new Guest*[newCap];
    for (int i = 0; i < numGuests; ++i) tmp[i] = guests[i];
    delete[] guests;
    guests = tmp;
    guestsCapacity = newCap;
}

void Cinema::resizeHalls() {
    int newCap = hallsCapacity * 2;
    Hall** tmp = new Hall*[newCap];
    for (int i = 0; i < numHalls; ++i) tmp[i] = halls[i];
    delete[] halls;
    halls = tmp;
    hallsCapacity = newCap;
}

void Cinema::resizeMovies() {
    int newCap = moviesCapacity * 2;
    Movie** tmp = new Movie*[newCap];
    for (int i = 0; i < numMovies; ++i) tmp[i] = movies[i];
    delete[] movies;
    movies = tmp;
    moviesCapacity = newCap;
}

void Cinema::resizeShifts() {
    int newCap = shiftsCapacity * 2;
    Shift** tmp = new Shift*[newCap];
    for (int i = 0; i < numShifts; ++i) tmp[i] = shifts[i];
    delete[] shifts;
    shifts = tmp;
    shiftsCapacity = newCap;
}

// ============================================================
//  Count getters
// ============================================================
int Cinema::getNumEmployees() const { return numEmployees; }
int Cinema::getNumGuests()    const { return numGuests;    }
int Cinema::getNumHalls()     const { return numHalls;     }
int Cinema::getNumMovies()    const { return numMovies;    }
int Cinema::getNumShifts()    const { return numShifts;    }

// ============================================================
//  By-index accessors (const and non-const overloads)
// ============================================================
const Employee* Cinema::getEmployeeByIndex(int i) const {
    if (i < 0 || i >= numEmployees) throw std::out_of_range("Employee index out of range.");
    return employees[i];
}
Employee* Cinema::getEmployeeByIndex(int i) {
    if (i < 0 || i >= numEmployees) throw std::out_of_range("Employee index out of range.");
    return employees[i];
}

const Guest* Cinema::getGuestByIndex(int i) const {
    if (i < 0 || i >= numGuests) throw std::out_of_range("Guest index out of range.");
    return guests[i];
}
Guest* Cinema::getGuestByIndex(int i) {
    if (i < 0 || i >= numGuests) throw std::out_of_range("Guest index out of range.");
    return guests[i];
}

const Hall* Cinema::getHallByIndex(int i) const {
    if (i < 0 || i >= numHalls) throw std::out_of_range("Hall index out of range.");
    return halls[i];
}
Hall* Cinema::getHallByIndex(int i) {
    if (i < 0 || i >= numHalls) throw std::out_of_range("Hall index out of range.");
    return halls[i];
}

const Movie* Cinema::getMovieByIndex(int i) const {
    if (i < 0 || i >= numMovies) throw std::out_of_range("Movie index out of range.");
    return movies[i];
}
Movie* Cinema::getMovieByIndex(int i) {
    if (i < 0 || i >= numMovies) throw std::out_of_range("Movie index out of range.");
    return movies[i];
}

const Shift* Cinema::getShiftByIndex(int i) const {
    if (i < 0 || i >= numShifts) throw std::out_of_range("Shift index out of range.");
    return shifts[i];
}

// ============================================================
//  Linear searches by ID
// ============================================================
Guest* Cinema::findGuestById(int id) {
    for (int i = 0; i < numGuests; ++i)
        if (guests[i]->getId() == id) return guests[i];
    return nullptr;
}

Employee* Cinema::findEmployeeById(int id) {
    for (int i = 0; i < numEmployees; ++i)
        if (employees[i]->getId() == id) return employees[i];
    return nullptr;
}

// ============================================================
//  operator+= — Cinema takes ownership of the heap object
// ============================================================
Cinema& Cinema::operator+=(Employee* e) {
    if (!e) throw std::invalid_argument("Employee pointer is null.");
    if (numEmployees == employeesCapacity) resizeEmployees();
    employees[numEmployees++] = e;
    return *this;
}

Cinema& Cinema::operator+=(Guest* g) {
    if (!g) throw std::invalid_argument("Guest pointer is null.");
    if (numGuests == guestsCapacity) resizeGuests();
    guests[numGuests++] = g;
    return *this;
}

Cinema& Cinema::operator+=(Hall* h) {
    if (!h) throw std::invalid_argument("Hall pointer is null.");
    if (numHalls == hallsCapacity) resizeHalls();
    halls[numHalls++] = h;
    return *this;
}

Cinema& Cinema::operator+=(Movie* m) {
    if (!m) throw std::invalid_argument("Movie pointer is null.");
    if (numMovies == moviesCapacity) resizeMovies();
    movies[numMovies++] = m;
    return *this;
}

Cinema& Cinema::operator+=(Shift* s) {
    if (!s) throw std::invalid_argument("Shift pointer is null.");
    if (numShifts == shiftsCapacity) resizeShifts();
    shifts[numShifts++] = s;
    return *this;
}

// ============================================================
//  operator-= — find guest by ID, delete it, compact the array
// ============================================================
Cinema& Cinema::operator-=(int guestId) {
    int idx = -1;
    for (int i = 0; i < numGuests; ++i) {
        if (guests[i]->getId() == guestId) { idx = i; break; }
    }
    if (idx == -1)
        throw std::invalid_argument("No guest found with the given ID.");

    delete guests[idx];
    for (int i = idx; i < numGuests - 1; ++i)
        guests[i] = guests[i + 1];
    guests[--numGuests] = nullptr;
    return *this;
}

// ============================================================
//  Print methods — call virtual printDetails()/printHall() for
//  polymorphic dispatch across the heterogeneous arrays
// ============================================================
void Cinema::printAllGuests() const {
    if (numGuests == 0) { std::cout << "  (no guests registered)" << std::endl; return; }
    for (int i = 0; i < numGuests; ++i) {
        std::cout << "[" << i << "] ";
        guests[i]->printDetails();  // virtual: Customer or Reviewer output
    }
}

void Cinema::printAllEmployees() const {
    if (numEmployees == 0) { std::cout << "  (no employees registered)" << std::endl; return; }
    for (int i = 0; i < numEmployees; ++i) {
        std::cout << "[" << i << "] ";
        employees[i]->printDetails();
    }
}

void Cinema::printAllHalls() const {
    if (numHalls == 0) { std::cout << "  (no halls added)" << std::endl; return; }
    for (int i = 0; i < numHalls; ++i) {
        std::cout << "[" << i << "] ";
        halls[i]->printHall();  // virtual: Hall / Hall3D / VIPHall / Hall3DVIP output
    }
}

void Cinema::printAllMovies() const {
    if (numMovies == 0) { std::cout << "  (no movies added)" << std::endl; return; }
    for (int i = 0; i < numMovies; ++i)
        std::cout << "[" << i << "] " << *movies[i] << std::endl;
}

void Cinema::printAllShifts() const {
    if (numShifts == 0) { std::cout << "  (no shifts scheduled)" << std::endl; return; }
    for (int i = 0; i < numShifts; ++i) {
        std::cout << "[" << i << "] ";
        shifts[i]->printShift();
    }
}
