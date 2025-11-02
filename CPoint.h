#pragma once
#include <iostream>
using namespace std;

class Point {
private:
	float xPoint; 
	float yPoint; 
public: 
	Point(float xT = 0, float yT = 0): xPoint(xT), yPoint(yT) {}
	Point(const Point& p) {
		xPoint = p.xPoint; 
		yPoint = p.yPoint; 
	}
	float distance(const Point& p); 
	void move(float xT, float yT); 
};