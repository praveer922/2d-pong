#include "gtest/gtest.h"
#include "Vector4.h"

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