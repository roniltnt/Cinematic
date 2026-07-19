#include <iostream>
#include <cstring>
#include <stdexcept>

#include "Cinema.h"
#include "Customer.h"
#include "Reviewer.h"
#include "Hall3D.h"
#include "VIPHall.h"
#include "Hall3DVIP.h"

// ============================================================
//  I/O Helpers  (file-scope only — no global variables)
// ============================================================
static int readIntInRange(const char* prompt, int lo, int hi) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(10000, '\n');
            if (val >= lo && val <= hi) return val;
            std::cout << "  Please enter a number between " << lo << " and " << hi << ".\n";
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Invalid input. Please enter an integer.\n";
        }
    }
}

static double readNonNegDouble(const char* prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= 0.0) {
            std::cin.ignore(10000, '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "  Invalid input. Please enter a non-negative number.\n";
    }
}

static void readString(const char* prompt, char* buf, int size) {
    std::cout << prompt;
    std::cin.getline(buf, size);
    if (!std::cin) { std::cin.clear(); buf[0] = '\0'; }
}

static bool readYesNo(const char* prompt) {
    char buf[8];
    while (true) {
        std::cout << prompt << " (y/n): ";
        std::cin.getline(buf, sizeof(buf));
        if (buf[0]=='y' || buf[0]=='Y') return true;
        if (buf[0]=='n' || buf[0]=='N') return false;
        std::cout << "  Please enter 'y' or 'n'.\n";
    }
}

static Date readDate(const char* header) {
    std::cout << header << '\n';
    int d = readIntInRange("  Day   (1-31)  : ", 1, 31);
    int m = readIntInRange("  Month (1-12)  : ", 1, 12);
    int y = readIntInRange("  Year          : ", 1900, 2100);
    return Date(d, m, y);
}

// ============================================================
//  Action implementations
// ============================================================

// Action 1 — Add Hall (no Movie; DEVIATION 1)
static void addHallAction(Cinema& cinema) {
    std::cout << "\n--- Add Hall ---\n";
    std::cout << "  Hall type: 1) Regular  2) 3D  3) VIP  4) 3D+VIP\n";
    int type = readIntInRange("  Choice: ", 1, 4);
    int num  = readIntInRange("  Hall number: ", 1, 9999);

    int glasses = 0;
    int waiters = 0;
    if (type == 2 || type == 4)
        glasses = readIntInRange("  Glasses count: ", 1, 9999);
    if (type == 3 || type == 4)
        waiters = readIntInRange("  Waiters count: ", 1, 999);

    try {
        Hall* h = nullptr;
        switch (type) {
            case 1: h = new Hall(num);                         break;
            case 2: h = new Hall3D(num, glasses);             break;
            case 3: h = new VIPHall(num, waiters);            break;
            case 4: h = new Hall3DVIP(num, waiters, glasses); break;
        }
        cinema += h;
        std::cout << "  Hall #" << num << " (" << h->getHallType() << ") added.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 2 — Add Movie (no premiere date; DEVIATION 1)
static void addMovieAction(Cinema& cinema) {
    std::cout << "\n--- Add Movie ---\n";
    char title[128];
    readString("  Title: ", title, sizeof(title));
    int  len  = readIntInRange("  Duration (minutes): ", 1, 600);
    bool is3D = readYesNo("  Is this a 3D movie?");

    try {
        cinema += new Movie(title, len, is3D);
        std::cout << "  Movie \"" << title << "\" added.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 3 — Schedule Screening (DEVIATION 2, 3)
static void scheduleScreeningAction(Cinema& cinema) {
    std::cout << "\n--- Schedule Screening ---\n";
    if (cinema.getNumMovies() == 0) { std::cout << "  No movies available.\n";  return; }
    if (cinema.getNumHalls()  == 0) { std::cout << "  No halls available.\n";   return; }

    std::cout << "\n  Movies:\n";
    cinema.printAllMovies();
    // DEVIATION FROM ORIGINAL SPEC: 1-based selection (Deviation 8)
    int mChoice = readIntInRange("  Select movie #: ", 1, cinema.getNumMovies()) - 1;

    std::cout << "\n  Halls:\n";
    cinema.printAllHalls();
    // DEVIATION FROM ORIGINAL SPEC: 1-based selection (Deviation 8)
    int hChoice = readIntInRange("  Select hall #: ", 1, cinema.getNumHalls()) - 1;

    Date date   = readDate("  Screening date:");
    int  hour   = readIntInRange("  Start hour   (0-23): ", 0, 23);
    int  minute = readIntInRange("  Start minute (0-59): ", 0, 59);

    // DEVIATION FROM ORIGINAL SPEC: 3D-match prompt if applicable (Deviation 2)
    bool is3D = false;
    if (cinema.canSchedule3D(mChoice, hChoice)) {
        is3D = readYesNo("  This hall supports 3D and the movie is 3D. Schedule as 3D?");
    } else {
        const Movie* m = cinema.getMovieByIndex(mChoice);
        const Hall*  h = cinema.getHallByIndex(hChoice);
        if (m->getIs3D() && h->get3DGlassesCount() == 0)
            std::cout << "  Note: movie is 3D but hall has no 3D glasses — scheduling as 2D.\n";
    }

    try {
        cinema.scheduleScreening(mChoice, hChoice, date, hour, minute, is3D);
        std::cout << "  Screening scheduled successfully.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 4 — Add Employee
static void addEmployeeAction(Cinema& cinema) {
    std::cout << "\n--- Add Employee ---\n";
    char name[128];
    readString("  Name: ", name, sizeof(name));
    int    id     = readIntInRange("  ID: ", 1, 999999);
    Date   bday   = readDate("  Birth date:");
    double salary = readNonNegDouble("  Salary: ");

    try {
        cinema += new Employee(name, id, bday, salary);
        std::cout << "  Employee \"" << name << "\" added.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 5 — Add Guest (Customer or Reviewer)
static void addGuestAction(Cinema& cinema) {
    std::cout << "\n--- Add Guest ---\n";
    std::cout << "  Guest type: 1) Customer  2) Reviewer\n";
    int type = readIntInRange("  Choice: ", 1, 2);

    char name[128];
    readString("  Name: ", name, sizeof(name));
    int id = readIntInRange("  ID: ", 1, 999999);
    // DEVIATION FROM ORIGINAL SPEC: visitCount not prompted; auto-initialized to 0 in constructor (Deviation 2)

    try {
        if (type == 1) {
            // DEVIATION FROM ORIGINAL SPEC: clubPoints removed — Customer takes only name and id (Deviation 1)
            cinema += new Customer(name, id);
        } else {
            char pub[128];
            readString("  Publication name: ", pub, sizeof(pub));
            // DEVIATION FROM ORIGINAL SPEC: visitCount parameter removed from Reviewer constructor (Deviation 2)
            cinema += new Reviewer(name, id, pub);
        }
        std::cout << "  Guest \"" << name << "\" added.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 6 — Sell Ticket (DEVIATION 5, 6)
static void sellTicketAction(Cinema& cinema) {
    std::cout << "\n--- Sell Ticket ---\n";
    if (cinema.getNumScreenings() == 0) { std::cout << "  No screenings scheduled.\n"; return; }
    if (cinema.getNumGuests()     == 0) { std::cout << "  No guests registered.\n";    return; }

    std::cout << "\n  Screenings:\n";
    cinema.printAllScreenings();
    // DEVIATION FROM ORIGINAL SPEC: 1-based selection (Deviation 8)
    int scChoice = readIntInRange("  Select screening #: ", 1, cinema.getNumScreenings()) - 1;

    const Screening* sc = cinema.getScreeningByIndex(scChoice);
    if (sc->getAvailableSeats() == 0) {
        std::cout << "  Sorry, this screening is fully booked.\n";
        return;
    }
    std::cout << "  Available seats: " << sc->getAvailableSeats() << '\n';

    std::cout << "\n  Guests:\n";
    cinema.printAllGuests();
    // DEVIATION FROM ORIGINAL SPEC: 1-based selection (Deviation 8)
    int gChoice = readIntInRange("  Select guest #: ", 1, cinema.getNumGuests()) - 1;

    // DEVIATION FROM ORIGINAL SPEC: ticket type (3D/VIP) auto-determined from hall/screening properties — no user prompt (Deviation 5)
    // DEVIATION FROM ORIGINAL SPEC: VIP ticket only issued when the hall is a VIP Hall (Deviation 3)
    try {
        cinema.sellTicket(scChoice, gChoice);
        std::cout << "  Ticket sold. Remaining seats: "
                  << cinema.getScreeningByIndex(scChoice)->getAvailableSeats() << '\n';
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 10 — Add Shift
static void addShiftAction(Cinema& cinema) {
    std::cout << "\n--- Add Shift ---\n";
    if (cinema.getNumEmployees() == 0) { std::cout << "  No employees registered.\n"; return; }

    std::cout << "\n  Employees:\n";
    cinema.printAllEmployees();
    // DEVIATION FROM ORIGINAL SPEC: 1-based selection (Deviation 8)
    int eChoice = readIntInRange("  Select employee #: ", 1, cinema.getNumEmployees()) - 1;
    int len     = readIntInRange("  Shift length (hours): ", 1, 24);
    Date date   = readDate("  Shift date:");

    try {
        cinema += new Shift(*cinema.getEmployeeByIndex(eChoice), len, date);
        std::cout << "  Shift added.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 11 — Promote Employee (DEVIATION 9)
static void promoteEmployeeAction(Cinema& cinema) {
    std::cout << "\n--- Promote Employee ---\n";
    if (cinema.getNumEmployees() == 0) { std::cout << "  No employees registered.\n"; return; }

    std::cout << "\n  Employees:\n";
    cinema.printAllEmployees();
    // DEVIATION FROM ORIGINAL SPEC: 1-based selection (Deviation 8)
    int eChoice = readIntInRange("  Select employee #: ", 1, cinema.getNumEmployees()) - 1;

    // DEVIATION FROM ORIGINAL SPEC: user enters percentage at runtime — NOT ++(*emp) (Deviation 9)
    double pct = readNonNegDouble("  Raise percentage (e.g. 10 for 10%): ");

    try {
        cinema.getEmployeeByIndex(eChoice)->promote(pct);
        std::cout << "  Employee promoted with a " << pct << "% raise.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// Action 13 — Delete Guest
static void deleteGuestAction(Cinema& cinema) {
    std::cout << "\n--- Delete Guest ---\n";
    if (cinema.getNumGuests() == 0) { std::cout << "  No guests registered.\n"; return; }

    std::cout << "\n  Guests:\n";
    cinema.printAllGuests();
    int id = readIntInRange("  Enter guest ID to remove: ", 0, 999999);

    try {
        cinema -= id;
        std::cout << "  Guest removed.\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << '\n';
    }
}

// ============================================================
//  Main — 14-action menu loop
// ============================================================
int main() {
    Cinema cinema;

    // DEVIATION FROM ORIGINAL SPEC: Exit moved to option 0 instead of 14 (Deviation 4)
    const char* MENU =
        "\n========================================\n"
        "       Smart Cinema Management\n"
        "========================================\n"
        " 1.  Add Hall\n"
        " 2.  Add Movie\n"
        " 3.  Schedule Screening\n"
        " 4.  Add Employee\n"
        " 5.  Add Guest\n"
        " 6.  Sell Ticket\n"
        " 7.  Print All Movies\n"
        " 8.  Print All Halls\n"
        " 9.  Print All Screenings\n"
        "10.  Add Shift\n"
        "11.  Promote Employee\n"
        "12.  Print All Guests\n"
        "13.  Delete Guest\n"
        "14.  Print All Employees\n"
        " 0.  Exit\n"  // DEVIATION FROM ORIGINAL SPEC: was 14, now 0 (Deviation 4)
        "========================================\n";

    while (true) {
        std::cout << MENU;
        // DEVIATION FROM ORIGINAL SPEC: range is 0-14; 0 = Exit (Deviation 4)
        int choice = readIntInRange("Choice: ", 0, 14);

        switch (choice) {
            case  0: std::cout << "Goodbye!\n"; return 0; // DEVIATION FROM ORIGINAL SPEC: Exit is 0 (Deviation 4)
            case  1: addHallAction(cinema);                             break;
            case  2: addMovieAction(cinema);                            break;
            case  3: scheduleScreeningAction(cinema);                   break;
            case  4: addEmployeeAction(cinema);                         break;
            case  5: addGuestAction(cinema);                            break;
            case  6: sellTicketAction(cinema);                          break;
            case  7: std::cout << '\n'; cinema.printAllMovies();        break;
            case  8: std::cout << '\n'; cinema.printAllHalls();         break;
            case  9: std::cout << '\n'; cinema.printAllScreenings();    break;
            case 10: addShiftAction(cinema);                            break;
            case 11: promoteEmployeeAction(cinema);                     break;
            case 12: std::cout << '\n'; cinema.printAllGuests();        break;
            case 13: deleteGuestAction(cinema);                         break;
            case 14: std::cout << '\n'; cinema.printAllEmployees();     break;
        }
    }
}
