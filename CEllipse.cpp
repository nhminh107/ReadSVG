#include "CEllipse.h"
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

void CEllipse::draw(sf::RenderWindow& window) const {
    // 1️⃣ Áp dụng ma trận biến đổi
    Point transformedCenter = center;
    transformedCenter.applyTransform(this->getFinalMatrix());

    // 2️⃣ Dùng bán trục thật
    float radiusX = ellipseWidth / 2.f;
    float radiusY = ellipseHeight / 2.f;

    // 3️⃣ Tạo hình tròn bán kính 1, rồi scale theo tỉ lệ ellipse
    sf::CircleShape ellipse(50.f); // 50 point detail để mịn
    ellipse.setPointCount(100);    // tăng độ mượt
    ellipse.setScale(radiusX / 50.f, radiusY / 50.f);

    // 4️⃣ Đặt vị trí
    ellipse.setPosition(transformedCenter.xPoint - radiusX,
        transformedCenter.yPoint - radiusY);

    // 5️⃣ Gán màu, viền
    ellipse.setFillColor(fillColor.to_sfml_color());
    ellipse.setOutlineColor(strokeColor.to_sfml_color());
    ellipse.setOutlineThickness(strokeWidth);

    // 6️⃣ Vẽ
    window.draw(ellipse);
}
