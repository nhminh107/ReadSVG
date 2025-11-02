#pragma once
#include "CShape.h"
#include "SVGTypes.h"
using namespace std;

class CCircle: public CShape {
private:
	float radius; 
	Point center; 
public: 
	CCircle(float r, float xC, float yC, const Color& fColor, const Color& sColor, float sWidth, const Matrix& m)
		: CShape(fColor, sColor, sWidth, m), center(Point(xC, yC)), radius(r) {}

	void draw(sf::RenderWindow& window) const override;
};