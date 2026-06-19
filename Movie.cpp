#include "Movie.h"
#include <cstring>
#include <stdexcept>

Movie::Movie(const char* title, const Date& premiereDate, int lengthMinutes, bool is3D)
    : premiereDate(premiereDate), lengthMinutes(lengthMinutes), is3D(is3D) {
    if (!title) throw std::invalid_argument("Movie title cannot be null.");
    if (lengthMinutes <= 0) throw std::invalid_argument("Movie length must be positive.");
    this->title = new char[strlen(title) + 1];
    strcpy(this->title, title);
}

Movie::Movie(const Movie& other)
    : premiereDate(other.premiereDate), lengthMinutes(other.lengthMinutes), is3D(other.is3D) {
    title = new char[strlen(other.title) + 1];
    strcpy(title, other.title);
}

Movie& Movie::operator=(const Movie& other) {
    if (this == &other) return *this;
    char* tmp = new char[strlen(other.title) + 1];
    strcpy(tmp, other.title);
    delete[] title;
    title        = tmp;
    premiereDate = other.premiereDate;
    lengthMinutes = other.lengthMinutes;
    is3D         = other.is3D;
    return *this;
}

Movie::~Movie() {
    delete[] title;
}

const char*  Movie::getTitle()          const { return title;         }
const Date&  Movie::getPremiereDate()   const { return premiereDate;  }
int          Movie::getLengthMinutes()  const { return lengthMinutes; }
bool         Movie::getIs3D()           const { return is3D;          }

void Movie::setTitle(const char* newTitle) {
    if (!newTitle) throw std::invalid_argument("Title cannot be null.");
    char* tmp = new char[strlen(newTitle) + 1];
    strcpy(tmp, newTitle);
    delete[] title;
    title = tmp;
}

void Movie::setLengthMinutes(int minutes) {
    if (minutes <= 0) throw std::invalid_argument("Length must be positive.");
    lengthMinutes = minutes;
}

void Movie::setIs3D(bool flag) { is3D = flag; }

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << "\"" << m.title << "\""
       << " | Premiere: " << m.premiereDate
       << " | " << m.lengthMinutes << " min"
       << " | 3D: " << (m.is3D ? "Yes" : "No");
    return os;
}
