#include "CEllipse.h"
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

void CEllipse::draw(sf::RenderWindow& window) const {
    const Matrix& m = this->getFinalMatrix();

    // 1️⃣ Áp dụng transform cho tâm ellipse
    Point transformedCenter = center;
    transformedCenter.applyTransform(m);

    // 2️⃣ Tính độ co giãn (scale) theo trục X và Y
    float scaleX = std::sqrt(m.m[0][0] * m.m[0][0] + m.m[0][1] * m.m[0][1]);
    float scaleY = std::sqrt(m.m[1][0] * m.m[1][0] + m.m[1][1] * m.m[1][1]);

    // 3️⃣ Bán trục sau khi scale
    float scaled_rx = (ellipseWidth / 2.0f) * scaleX;
    float scaled_ry = (ellipseHeight / 2.0f) * scaleY;

    // 4️⃣ Tạo hình ellipse bằng CircleShape (vì SFML không có ellipse)
    sf::CircleShape ellipse(100.0f); // base radius (100 chỉ là tỉ lệ gốc)
    ellipse.setPointCount(100);

    // 5️⃣ Scale từ hình tròn sang ellipse thật
    ellipse.setScale(scaled_rx / 100.0f, scaled_ry / 100.0f);

    // 6️⃣ Đặt origin và vị trí
    ellipse.setOrigin(100.0f, 100.0f);
    ellipse.setPosition(transformedCenter.xPoint, transformedCenter.yPoint);

    // 7️⃣ Tính stroke theo tỉ lệ trung bình (giống CCircle)
    float avgScale = (scaleX + scaleY) / 2.0f;
    ellipse.setOutlineThickness(strokeWidth * avgScale);

    // 8️⃣ Gán màu
    ellipse.setFillColor(fillColor.to_sfml_color());
    ellipse.setOutlineColor(strokeColor.to_sfml_color());

    // 9️⃣ Vẽ
    window.draw(ellipse);
}
