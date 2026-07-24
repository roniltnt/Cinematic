#ifndef REVIEWER_H
#define REVIEWER_H

#include "Guest.h"

class Reviewer : public Guest {
private:
    char* publicationName;

public:
    // DEVIATION FROM ORIGINAL SPEC: visitCount parameter removed; auto-initialized to 0 (Deviation 2)
    Reviewer(const char* name, int id, const char* publicationName);
    Reviewer(const Reviewer& other);
    Reviewer& operator=(const Reviewer& other);
    ~Reviewer() override;

    const char* getPublicationName() const;
    void setPublicationName(const char* name);

    void printDetails() const override;
    Reviewer* clone() const override; // CHANGED: covariant clone for polymorphic Guest copy
};

#endif
