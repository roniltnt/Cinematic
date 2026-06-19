#include "Person.h"
#include <cstring>
#include <stdexcept>

Person::Person(const char* name, int id) : id(id) {
    if (!name) throw std::invalid_argument("Person name cannot be null.");
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

Person::Person(const Person& other) : id(other.id) {
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
}

Person& Person::operator=(const Person& other) {
    if (this == &other) return *this;
    // Allocate first so a failed new() leaves *this intact
    char* tmp = new char[strlen(other.name) + 1];
    strcpy(tmp, other.name);
    delete[] name;
    name = tmp;
    id = other.id;
    return *this;
}

Person::~Person() {
    delete[] name;
}

const char* Person::getName() const { return name; }
int          Person::getId()  const { return id;   }

void Person::setName(const char* newName) {
    if (!newName) throw std::invalid_argument("Name cannot be null.");
    char* tmp = new char[strlen(newName) + 1];
    strcpy(tmp, newName);
    delete[] name;
    name = tmp;
}
