#include "CEllipse.h"
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

void CEllipse::draw(sf::RenderWindow& window) const {
    Matrix m = this->getFinalMatrix();

    // Tâm đã transform
    Point centerT = center;
    centerT.applyTransform(m);

    // Tính bán trục X thật: transform (cx + rx, cy)
    float rx = ellipseWidth / 2.0f;
    float ry = ellipseHeight / 2.0f;

    Point px(center.xPoint + rx, center.yPoint);
    px.applyTransform(m);
    float scaled_rx = px.distance(centerT);

    Point py(center.xPoint, center.yPoint + ry);
    py.applyTransform(m);
    float scaled_ry = py.distance(centerT);

    // Dùng bán trục đã scale để dựng ellipse.
    // Tạo CircleShape với radius base và scale theo tỉ lệ (an toàn)
    float base = 100.f; // base radius for circle shape
    sf::CircleShape circle(base);
    circle.setPointCount(100);

    // scale so that base * scaleX = scaled_rx
    float scaleX = (base > 0.f) ? (scaled_rx / base) : 1.f;
    float scaleY = (base > 0.f) ? (scaled_ry / base) : 1.f;
    circle.setScale(scaleX, scaleY);

    // vị trí: đặt top-left
    circle.setPosition(centerT.xPoint - scaled_rx, centerT.yPoint - scaled_ry);

    // stroke thickness scale
    float sX = fabs(m.m[0][0]);
    float sY = fabs(m.m[1][1]);
    float avgS = (sX + sY) * 0.5f;
    circle.setOutlineThickness(strokeWidth * avgS);

    circle.setFillColor(fillColor.to_sfml_color());
    circle.setOutlineColor(strokeColor.to_sfml_color());

    window.draw(circle);
}
