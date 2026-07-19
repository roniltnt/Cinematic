#ifndef TICKET_H
#define TICKET_H

// DEVIATION FROM ORIGINAL SPEC: Ticket now references a Screening instead of a Movie
// (Deviation 5). calcFinalPrice() and operator> removed entirely (Deviation 4).
class Screening; // forward declaration — avoids circular include chain

class Ticket {
private:
    const Screening& screeningRef; // DEVIATION FROM ORIGINAL SPEC: was const Movie&
    bool is3D;

public:
    // CHANGED: constructor now takes Screening reference
    Ticket(const Screening& screening, bool is3D);
    Ticket(const Ticket& other);
    Ticket& operator=(const Ticket& other) = delete; // reference member prevents assignment
    virtual ~Ticket();

    const Screening& getScreening() const;
    bool getIs3D() const;
    void setIs3D(bool flag);

    // CHANGED: replaces calcFinalPrice(); prints ticket summary to stdout
    virtual void printTicket() const;
    virtual Ticket* clone() const;
};

#endif
