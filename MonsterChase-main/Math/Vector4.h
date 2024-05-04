#pragma once

#include <iostream>

class Vector4 {

public:
    float x, y, z, w;
    // Constructors
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector4& other);

    // Length of the vector
    float length() const;

    // Normalize the vector
    void normalize();

    // Dot product
    float dot(const Vector4& other) const;

    // Overloaded operators
    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4& operator+=(const Vector4& other);
    Vector4& operator-=(const Vector4& other);
    Vector4& operator=(const Vector4& other);
    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    // Display function
    friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
};

