#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdint>
#include <sstream>

using namespace std;
const int MATRIX_SIZE = 3;

struct Color {
    //range from 0->255
    int r;
    int g;
    int b;
    int a; //Capacity 

    Color(int red = 0, int green = 0, int blue = 0, int capacity = 10) : r(red), g(green), b(blue), a(capacity) {};
    Color(const Color& col) {
        r = col.r; 
        g = col.g; 
        b = col.b; 
        a = col.a; 
    }
    sf::Color to_sfml_color() const {
        return sf::Color(
            static_cast<uint8_t>(min(255, max(0, r))),
            static_cast<uint8_t>(min(255, max(0, g))),
            static_cast<uint8_t>(min(255, max(0, b))),
            static_cast<uint8_t>(min(255, max(0, a)))
        );
    }
};




struct Matrix {
    float m[MATRIX_SIZE][MATRIX_SIZE];

    Matrix() {
        // Khởi tạo ma trận đơn vị (I): đường chéo là 1, còn lại là 0
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = 0; j < MATRIX_SIZE; ++j) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    Matrix(const Matrix& other) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                m[i][j] = other.m[i][j];
            }
        }
    }

    // Phương thức tĩnh: Trả về một ma trận đơn vị
    static Matrix Identity() {
        return Matrix();
    }

    // Hàm áp dụng phép tịnh tiến (Translation)
    static Matrix createTranslation(float tx, float ty) {
        Matrix result = Identity();
        result.m[0][2] = tx;
        result.m[1][2] = ty;
        return result;
    }

    static Matrix createScale(float sx, float sy) {
        Matrix result = Matrix::Identity();
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        return result;
    }

    // Hàm quan trọng nhất: Nhân ma trận (Matrix Concatenation)
    // Cần thiết để áp dụng chuỗi phép biến đổi, ví dụ: transform="translate(x) rotate(a)"
    Matrix multiply(const Matrix& other) const {
        Matrix result = Matrix(); // Khởi tạo kết quả bằng ma trận đơn vị

        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = 0; j < MATRIX_SIZE; ++j) {
                result.m[i][j] = 0.0f;
                // Phép nhân hàng i của ma trận này với cột j của ma trận 'other'
                for (int k = 0; k < MATRIX_SIZE; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }


    static Matrix createRotation(float angle_degrees) {

        float angle_radians = angle_degrees * (3.14f / 180.0f);
        Matrix result = Identity();
        float cosA = cos(angle_radians);
        float sinA = sin(angle_radians);

        //Phép xoay 2D cần chừng này thôi

        result.m[0][0] = cosA;
        result.m[0][1] = -sinA;
        result.m[1][0] = sinA;
        result.m[1][1] = cosA;
        return result; 
    }   
};

class Point {
public:
    float xPoint;
    float yPoint;
public:
    Point(float xT = 0, float yT = 0) : xPoint(xT), yPoint(yT) {}
    Point(const Point& p) {
        xPoint = p.xPoint;
        yPoint = p.yPoint;
    }
    float distance(const Point& p) const;
    void applyTransform(const Matrix& m);
    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const; 

};