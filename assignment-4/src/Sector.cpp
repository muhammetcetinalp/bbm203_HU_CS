#include "Sector.h"
#include <sstream>
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    std::string message;
    int rounded_number = static_cast<int>(distance_from_earth);
    std::stringstream ss;
    ss << rounded_number;
    message += ss.str();

    if(x == 0){
        message += 'S';
    }
    else if(x > 0){
        message += 'R';
    }
    else if(x < 0){
        message += 'L';
    }

    if(y == 0){
        message += 'S';
    }
    else if(y > 0){
        message += 'U';
    }
    else if(y < 0){
        message += 'D';
    }

    if(z == 0){
        message += 'S';
    }
    else if(z > 0){
        message += 'F';
    }
    else if(z < 0){
        message += 'B';
    }
    sector_code = message;

}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}