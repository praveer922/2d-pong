#include "Matrix4.h"
#include <cstring>

// Constructors
Matrix4::Matrix4() {
    std::memset(data, 0, sizeof(data));
}

Matrix4::Matrix4(float diagonal) {
    std::memset(data, 0, sizeof(data));
    for (int i = 0; i < 4; ++i) {
        data[i][i] = diagonal;
    }
}

Matrix4::Matrix4(float newData[4][4]) {
    std::memcpy(data, newData, sizeof(data));
}

// Constructor with nested initializer list
Matrix4::Matrix4(std::initializer_list<std::initializer_list<float>> list) {
    int i = 0, j = 0;
    for (const auto& row : list) {
        j = 0;
        for (const auto& val : row) {
            data[i][j++] = val;
        }
        ++i;
    }
}

// Destructor
Matrix4::~Matrix4() {}

// Overloaded [] operator for getting and setting elements
float* Matrix4::operator[](int index) {
    return data[index];
}

const float* Matrix4::operator[](int index) const {
    return data[index];
}

// Overloaded operators
Matrix4 Matrix4::operator+(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix4& Matrix4::operator+=(const Matrix4& other) {
    *this = *this + other;
    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& other) {
    *this = *this - other;
    return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& other) {
    *this = *this * other;
    return *this;
}

Matrix4& Matrix4::operator=(const Matrix4& other) {
    if (this != &other) {
        std::memcpy(data, other.data, sizeof(data));
    }
    return *this;
}

bool Matrix4::operator==(const Matrix4& other) const {
    return std::memcmp(data, other.data, sizeof(data)) == 0;
}

bool Matrix4::operator!=(const Matrix4& other) const {
    return !(*this == other);
}

// Display function
std::ostream& operator<<(std::ostream& os, const Matrix4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            os << matrix.data[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}
