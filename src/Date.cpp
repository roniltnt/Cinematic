#include "../include/Date.h"
#include <stdexcept>

bool Date::isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (leap) daysInMonth[1] = 29;
    return d <= daysInMonth[m - 1];
}

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {
    if (!isValidDate(day, month, year))
        throw std::invalid_argument("Invalid date: values out of range.");
}

int Date::getDay()   const { return day;   }
int Date::getMonth() const { return month; }
int Date::getYear()  const { return year;  }

void Date::setDay(int d) {
    if (!isValidDate(d, month, year)) throw std::invalid_argument("Invalid day value.");
    day = d;
}
void Date::setMonth(int m) {
    if (!isValidDate(day, m, year)) throw std::invalid_argument("Invalid month value.");
    month = m;
}
void Date::setYear(int y) {
    if (!isValidDate(day, month, y)) throw std::invalid_argument("Invalid year value.");
    year = y;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.day << "/" << d.month << "/" << d.year;
    return os;
}
