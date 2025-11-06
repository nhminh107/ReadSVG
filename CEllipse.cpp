// ============ ELLIPSE.CPP ============
#include "CEllipse.h"
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

void CEllipse::draw(sf::RenderWindow& window) const {
    const Matrix& m = this->getFinalMatrix();

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

    // Tạo CircleShape với base radius
    float base = 100.0f;
    sf::CircleShape circle(base);
    circle.setPointCount(100);

    // Scale để khớp với bán trục đã transform
    float scaleX = (base > 0.0f) ? (scaled_rx / base) : 1.0f;
    float scaleY = (base > 0.0f) ? (scaled_ry / base) : 1.0f;
    circle.setScale(scaleX, scaleY);

    // Đặt origin tại tâm (giống Circle)
    circle.setOrigin(base, base);
    circle.setPosition(centerT.xPoint, centerT.yPoint);

    // SỬA: Tính average scale giống Circle
    float avgScale = (scaled_rx / rx + scaled_ry / ry) * 0.5f;
    circle.setOutlineThickness(strokeWidth * avgScale);

    circle.setFillColor(fillColor.to_sfml_color());
    circle.setOutlineColor(strokeColor.to_sfml_color());

    window.draw(circle);
}