#include "gtest/gtest.h"
#include "Vector4.h"
#include "Matrix4.h"

// Test length calculation
TEST(Vector4Test, Length) {
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.length(), 5.4772258f); // Approximate value due to floating-point precision
}

// Test normalization
TEST(Vector4Test, Normalize) {
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.length(), 1.0f);
}

// Test dot product
TEST(Vector4Test, DotProduct) {
    Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
    EXPECT_FLOAT_EQ(v1.dot(v2), 40.0f);
}

// Test addition operator
TEST(Vector4Test, AdditionOperator) {
    Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
    Vector4 result = v1 + v2;
    EXPECT_EQ(result, Vector4(3.0f, 5.0f, 7.0f, 9.0f));
}

// Test subtraction operator
TEST(Vector4Test, SubtractionOperator) {
    Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
    Vector4 result = v2 - v1;
    EXPECT_EQ(result, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

// Test constructor and equality operator
TEST(Matrix4Test, ConstructorAndEquality) {
    Matrix4 m1;
    Matrix4 m2(1.0f);
    Matrix4 m3({
        {1.0f, 2.0f, 3.0f, 4.0f},
        {5.0f, 6.0f, 7.0f, 8.0f},
        {9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
        });

    // Test equality
    EXPECT_TRUE(m1 == m1);
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m2 == m2);
    EXPECT_TRUE(m3 == m3);
}

// Test addition operator
TEST(Matrix4Test, AdditionOperator) {
    Matrix4 m1({
        {1.0f, 2.0f, 3.0f, 4.0f},
        {5.0f, 6.0f, 7.0f, 8.0f},
        {9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
        });
    Matrix4 m2({
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f}
        });
    Matrix4 expected({
        {2.0f, 3.0f, 4.0f, 5.0f},
        {6.0f, 7.0f, 8.0f, 9.0f},
        {10.0f, 11.0f, 12.0f, 13.0f},
        {14.0f, 15.0f, 16.0f, 17.0f}
        });

    Matrix4 result = m1 + m2;
    EXPECT_EQ(result, expected);
}

// Test multiplication operator
TEST(Matrix4Test, MultiplicationOperator) {
    Matrix4 m1({
        {1.0f, 2.0f, 3.0f, 4.0f},
        {5.0f, 6.0f, 7.0f, 8.0f},
        {9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
        });
    Matrix4 m2({
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
        });

    Matrix4 result = m1 * m2;
    EXPECT_EQ(result, m1);
}