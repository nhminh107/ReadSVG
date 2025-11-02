#include "CCircle.h"
using namespace std;

void CCircle::draw(sf::RenderWindow& window) const {
    // 1️⃣ Tạo bản sao tâm
    Point transformedCenter = center;

    // 2️⃣ Áp dụng ma trận biến đổi
    transformedCenter.applyTransform(this->getFinalMatrix());

    // 3️⃣ Vẽ bằng SFML
    sf::CircleShape circle(radius);
    circle.setPosition(transformedCenter.xPoint - radius, transformedCenter.yPoint - radius);
    circle.setFillColor(fillColor.to_sfml_color());
    circle.setOutlineColor(strokeColor.to_sfml_color());
    circle.setOutlineThickness(strokeWidth);

    window.draw(circle);
}
