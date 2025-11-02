#pragma once
#include "CShape.h"
#include "SVGTypes.h"
using namespace std;

class CRectangle : public CShape {
private:
	float width; 
	float height; 
	Point topLeft; 
public: 
	CRectangle(float w, float h, float xRect, float yRect, const Color& fColor, const Color& sColor, const Matrix& m, float sBorder) :
		CShape(fColor, sColor, sBorder, m), topLeft(Point(xRect, yRect)), width(w), height(h) {}
	void draw(sf::RenderWindow& window) const override;
};
