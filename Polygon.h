#pragma once
#include "CShape.h"
#include "SVGTypes.h"
#include <vector>
using namespace std;

class CPolygon : public CShape {
private:
    vector<Point> pointList;
    bool isClosed; // Thêm cờ nhận biết polygon hay polyline

public:
    CPolygon(const vector<Point>& pList, float sBorder, const Color& fColor,
        const Color& sColor, const Matrix& m, bool closed = true)
        : CShape(fColor, sColor, sBorder, m),
        pointList(pList),
        isClosed(closed) {}

    void draw(sf::RenderWindow& window) const override;
};
