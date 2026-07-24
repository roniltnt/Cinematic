#include "../include/Reviewer.h"
#include <iostream>
#include <cstring>
#include <stdexcept>

// DEVIATION FROM ORIGINAL SPEC: visitCount parameter removed; Guest auto-initializes to 0 (Deviation 2)
Reviewer::Reviewer(const char* name, int id, const char* publicationName)
    : Guest(name, id) {
    if (!publicationName) throw std::invalid_argument("Publication name cannot be null.");
    this->publicationName = new char[strlen(publicationName) + 1];
    strcpy(this->publicationName, publicationName);
}

Reviewer::Reviewer(const Reviewer& other)
    : Guest(other) {
    publicationName = new char[strlen(other.publicationName) + 1];
    strcpy(publicationName, other.publicationName);
}

Reviewer& Reviewer::operator=(const Reviewer& other) {
    if (this == &other) return *this;
    Guest::operator=(other);
    char* tmp = new char[strlen(other.publicationName) + 1];
    strcpy(tmp, other.publicationName);
    delete[] publicationName;
    publicationName = tmp;
    return *this;
}

Reviewer::~Reviewer() {
    delete[] publicationName;
}

const char* Reviewer::getPublicationName() const { return publicationName; }

void Reviewer::setPublicationName(const char* name) {
    if (!name) throw std::invalid_argument("Publication name cannot be null.");
    char* tmp = new char[strlen(name) + 1];
    strcpy(tmp, name);
    delete[] publicationName;
    publicationName = tmp;
}

void Reviewer::printDetails() const {
    std::cout << "Reviewer: "       << getName()
              << " | ID: "           << getId()
              << " | Visits: "       << getVisitCount()
              << " | Tickets: "      << getNumTickets()
              << " | Publication: "  << publicationName << std::endl;
}

Reviewer* Reviewer::clone() const {
    return new Reviewer(*this);
}
