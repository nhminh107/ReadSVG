#include "SVGTypes.h"
using namespace std; 

float Point::distance(const Point& p) const {
	float x = (p.xPoint - xPoint) * (p.xPoint - xPoint);
	float y = (p.yPoint - yPoint) * (p.yPoint - yPoint);
	return sqrt(x + y); 
}
Point Point::operator+(const Point& other) const {
	return Point(xPoint + other.xPoint, yPoint + other.yPoint);
}
Point Point::operator-(const Point& other) const {
	return Point(xPoint - other.xPoint, yPoint - other.yPoint);
}


void Point::applyTransform(const Matrix& m) {
    float original_x = xPoint;
    float original_y = yPoint;

    float x_new = m.m[0][0] * original_x +
        m.m[0][1] * original_y +
        m.m[0][2] * 1.0f;

    float y_new = m.m[1][0] * original_x +
        m.m[1][1] * original_y +
        m.m[1][2] * 1.0f;


    xPoint = x_new;
    yPoint = y_new;
}