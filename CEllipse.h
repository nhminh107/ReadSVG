#pragma once
#include "CShape.h"
#include "SVGTypes.h"
using namespace std;

class CEllipse : public CShape {
private: 
	Point center; 
	float ellipseWidth; 
	float ellipseHeight; 
public:
    CEllipse(
        const Point& centerPoint,
        float width,
        float height,
        const Color& fColor,
        const Color& sColor,
        float sWidth,
        const Matrix& fMatrix
    ) :
        center(centerPoint),
        ellipseWidth(width),
        ellipseHeight(height),
        CShape(fColor, sColor, sWidth, fMatrix)
    {}

    void draw(sf::RenderWindow& window) const override;
};
