#include "Vector4.h"
#include <cmath>

// Constructors
Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4::Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

// Length of the vector
float Vector4::length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

// Normalize the vector
void Vector4::normalize() {
    float len = length();
    if (len != 0.0f) {
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }
}

// Dot product
float Vector4::dot(const Vector4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

// Overloaded operators
Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4& Vector4::operator+=(const Vector4& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vector4& Vector4::operator=(const Vector4& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    return *this;
}

bool Vector4::operator==(const Vector4& other) const {
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

bool Vector4::operator!=(const Vector4& other) const {
    return !(*this == other);
}

// Display function
std::ostream& operator<<(std::ostream& os, const Vector4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}
