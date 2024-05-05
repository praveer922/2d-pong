#pragma once

#include <iostream>

class Matrix4 {
private:
    float data[4][4];

public:
    // Constructors
    Matrix4();
    Matrix4(float diagonal);
    Matrix4(float newData[4][4]);
    Matrix4::Matrix4(std::initializer_list<std::initializer_list<float>> list);

    // Destructor
    ~Matrix4();

    // Overloaded [] operator for getting and setting elements
    float* operator[](int index);
    const float* operator[](int index) const;

    // Overloaded operators
    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator-(const Matrix4& other) const;
    Matrix4 operator*(const Matrix4& other) const;
    Matrix4& operator+=(const Matrix4& other);
    Matrix4& operator-=(const Matrix4& other);
    Matrix4& operator*=(const Matrix4& other);
    Matrix4& operator=(const Matrix4& other);
    bool operator==(const Matrix4& other) const;
    bool operator!=(const Matrix4& other) const;

    // Display function
    friend std::ostream& operator<<(std::ostream& os, const Matrix4& matrix);
};

