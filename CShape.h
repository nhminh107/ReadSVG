#pragma once
#include "SVGTypes.h"
using namespace std;

class CShape {
protected: 
	Color fillColor; 
	Color strokeColor; 
	float strokeWidth;
	Matrix finalMatrix; 
public: 
	CShape(const Color& fColor, const Color& sColor, float sWidth, const Matrix& fMatrix):
		fillColor(fColor), strokeColor(sColor), strokeWidth(sWidth), finalMatrix(fMatrix) {}

	virtual void draw(sf::RenderWindow& window) const = 0;
	virtual ~CShape() = default;
	void setFinalMatrix(const Matrix& m) { finalMatrix = m; }
	const Matrix& getFinalMatrix() const;
};