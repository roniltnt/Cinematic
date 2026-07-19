#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>

// DEVIATION FROM ORIGINAL SPEC: Movie no longer carries a premiere Date.
// Only title, duration, and 3D flag are stored (Deviation 1).
class Movie {
private:
    char* title;
    int lengthMinutes;
    bool is3D;

public:
    // CHANGED: removed Date parameter from constructor (Deviation 1)
    Movie(const char* title, int lengthMinutes, bool is3D);
    Movie(const Movie& other);
    Movie& operator=(const Movie& other);
    ~Movie();

    const char* getTitle()        const;
    int         getLengthMinutes() const;
    bool        getIs3D()         const;

    void setTitle(const char* newTitle);
    void setLengthMinutes(int minutes);
    void setIs3D(bool flag);

    friend std::ostream& operator<<(std::ostream& os, const Movie& m);
};

#endif
