#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <cstdlib>

Vector2 setMag(Vector2 v, float length) {
    float magOfCurr = sqrt(v.x * v.x + v.y * v.y);
    if (magOfCurr < length) {
        return Vector2{ 0,0 };
    }
    return Vector2{ (v.x / magOfCurr) * length,(v.y / magOfCurr) * length };
}