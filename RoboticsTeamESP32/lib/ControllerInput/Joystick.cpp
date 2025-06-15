// Joystick.cpp
#include "Joystick.h"

Joystick::Joystick(float xVal, float yVal) : x(xVal), y(yVal) {
    calculateMetrics(); // Calculate magnitude and angle immediately
}

void Joystick::setPosition(float xVal, float yVal) {
    x = xVal;
    y = yVal;
    calculateMetrics(); // Update magnitude and angle after setting new position
}

void Joystick::calculateMetrics() {
    magnitude = std::sqrt(x * x + y * y);
    float angleRad = std::atan2(y, x);
    angleDegrees = angleRad * (180.0f / M_PI);

    // Optional: Handle deadzone or clamping if needed
    // if (magnitude < deadzone) {
    //     magnitude = 0.0f;
    //     angleDegrees = 0.0f; // Or leave it as is, depending on desired behavior
    // }
    // if (magnitude > maxMagnitude) {
    //    // Scale x and y down or normalize magnitude
    // }
}