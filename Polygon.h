#pragma once
#include "CShape.h"
#include "SVGTypes.h"
#include <vector>
using namespace std;

class CPolygon : public CShape {
private:
	vector<Point> pointList;
public:

	CPolygon(const vector<Point>& pList, float sBorder, const Color& fColor, const Color& sColor, const Matrix& m) :
		CShape(fColor, sColor, sBorder, m), pointList(pList) {}
	void draw(sf::RenderWindow& window) const override;

};