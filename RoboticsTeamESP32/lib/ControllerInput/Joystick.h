// Joystick.h
#pragma once // Or #ifndef/#define/#endif guards

#include <cmath> // For sqrt, atan2

#ifndef M_PI
#define M_PI 3.14159265358979323846f // Define M_PI if it's not available
#endif

class Joystick {
private:
    float x;
    float y;
    float magnitude;
    float angleDegrees;

    // Helper to update magnitude and angle
    void calculateMetrics();

public:
    Joystick(float xVal = 0.0f, float yVal = 0.0f); // Add default values for convenience
    void setPosition(float xVal, float yVal);

    // Getter methods
    float getX() const { return x; }
    float getY() const { return y; }
    float getMagnitude() const { return magnitude; }
    float getAngleDegrees() const { return angleDegrees; }
};