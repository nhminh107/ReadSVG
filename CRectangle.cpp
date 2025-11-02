#include "CRectangle.h"
using namespace std;

void CRectangle::draw(sf::RenderWindow& window) const {
    Matrix m = this->getFinalMatrix();

    // Tọa độ 4 góc ban đầu
    Point p0 = topLeft;
    Point p1 = Point(topLeft.xPoint + width, topLeft.yPoint);
    Point p2 = Point(topLeft.xPoint + width, topLeft.yPoint + height);
    Point p3 = Point(topLeft.xPoint, topLeft.yPoint + height);

    p0.applyTransform(m);
    p1.applyTransform(m);
    p2.applyTransform(m);
    p3.applyTransform(m);

    // Nếu có rotation / shear, ta không thể chỉ dùng width/height đơn giản.
    // Ta vẽ bằng sf::ConvexShape với 4 đỉnh:
    sf::ConvexShape rect;
    rect.setPointCount(4);
    rect.setPoint(0, sf::Vector2f(p0.xPoint, p0.yPoint));
    rect.setPoint(1, sf::Vector2f(p1.xPoint, p1.yPoint));
    rect.setPoint(2, sf::Vector2f(p2.xPoint, p2.yPoint));
    rect.setPoint(3, sf::Vector2f(p3.xPoint, p3.yPoint));

    rect.setFillColor(fillColor.to_sfml_color());
    rect.setOutlineColor(strokeColor.to_sfml_color());

    // stroke thickness: ước lượng scale như trên
    float sX = fabs(m.m[0][0]);
    float sY = fabs(m.m[1][1]);
    float avgS = (sX + sY) * 0.5f;
    rect.setOutlineThickness(strokeWidth * avgS);

    window.draw(rect);
}
