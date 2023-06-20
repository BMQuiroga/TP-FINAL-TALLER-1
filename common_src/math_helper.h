#ifndef _MATHHELPER_H
#define _MATHHELPER_H

#include <math.h>
#include <cmath>
#include <random>


#define PI 3.14159265
#define DEG_TO_RAD PI / 180.0f
#define RAD_TO_DEG 180.0f / PI
//--------------------------------------------------------------------------------------------------
// Vector2D structs consist of x and y values discribing a 2D vector,                                
// along with vector magnitude calculations, normalization, and operator overloading of += and -=   
//--------------------------------------------------------------------------------------------------
struct Vector2D {
    float x;
    float y;

    Vector2D(float _x = 0.0f, float _y = 0.0f)
        : x(_x), y(_y) {}

    //Calculates the square of the magnitude (preferably used instead of magnitude if possible)   
    float magnitude_squared() {
        return x*x + y*y;
    }

    //Calculates the magnitude of the vector     
    float magnitude() {
        return (float)sqrt(x*x + y*y);
    }

    //Returns a directional Vector2D with the same direction as the Vector2D but of length 1    
    // (Does not change the x and y values of the original vector)
    Vector2D normalized() {
        float mag = magnitude();

        return Vector2D(x / mag, y / mag);
    }

    Vector2D& operator +=(const Vector2D& rhs) {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    Vector2D& operator -=(const Vector2D& rhs) {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    Vector2D operator -() const {
        return Vector2D(-x, -y);
    }
};

inline Vector2D operator +(const Vector2D& lhs, const Vector2D& rhs) {
    return Vector2D(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vector2D operator -(const Vector2D& lhs, const Vector2D& rhs) {
    return Vector2D(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vector2D operator *(const Vector2D& lhs, const float& rhs) {
    return Vector2D(lhs.x * rhs, lhs.y * rhs);
}

inline Vector2D operator *(const float& lhs, const Vector2D& rhs) {
    return Vector2D(lhs * rhs.x, lhs * rhs.y);
}


//Rotates the given vector by the given angle around the origin   
//(Does not change the original vector)
inline Vector2D rotate(Vector2D vec, float angle) {
    //converting the angle to radians to be used in sin and cos functions
    float radAngle = (float)(angle*DEG_TO_RAD);
    
    return Vector2D((float)(vec.x * cos(radAngle) - vec.y * sin(radAngle)), (float)(vec.x * sin(radAngle) + vec.y * cos(radAngle)));
}

inline Vector2D lerp(Vector2D& start, Vector2D& end, float time)
{
    if (time <= 0.0f)
    {
        return start;
    }
    if (time >= 1.0f)
    {
        return end;
    }
    Vector2D dir = (end - start).normalized();
    float mag = (end - start).magnitude();
    return start + dir * mag * time;
}

inline int calculateDistance(Vector2D a, Vector2D b) {
    double deltaX = a.x - b.x;
    double deltaY = a.y - b.y;
    double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    return distance;
}

// Function to generate a random number within a given range
inline int getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}


const Vector2D VEC2_ZERO = { 0.0f, 0.0f };
const Vector2D VEC2_ONE = { 1.0f, 1.0f };
const Vector2D VEC2_UP = { 0.0f, 1.0f };
const Vector2D VEC2_RIGHT = { 1.0f, 0.0f };
#endif
