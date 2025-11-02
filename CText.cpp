#include "CText.h"
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

void CText::draw(sf::RenderWindow& window) const {
    // 1. Transform vị trí
    Point transformedPlace = startPoint;
    Matrix m = this->getFinalMatrix();
    transformedPlace.applyTransform(m);

    // 2. TẠO ĐỐI TƯỢNG SFML
    sf::Text text;

    // NOTE: Bạn phải đảm bảo m_font đã được tải thành công trong loadFont()
    if (m_font.getInfo().family.empty()) return;
    text.setFont(m_font);

    // 3. SCALE VÀ SIZE
    float scaleX = fabs(m.m[0][0]);
    float scaleY = fabs(m.m[1][1]);
    float avgScale = (scaleX + scaleY) / 2.f;

    text.setString(content);
    // Áp dụng scale cho fontSize
    text.setCharacterSize(static_cast<unsigned int>(fontSize * avgScale));

    // 4. ĐỊNH VỊ (Sửa lỗi Baseline và Origin)
    sf::FloatRect bounds = text.getLocalBounds();

    // Đặt gốc X vào lề trái (bounds.left) và gốc Y vào đường cơ sở (Baseline)
    text.setOrigin(bounds.left, bounds.top + bounds.height);

    // Đặt vị trí theo tọa độ (x, y) SVG đã biến đổi
    text.setPosition(transformedPlace.xPoint, transformedPlace.yPoint);

    // 5. THIẾT LẬP KIỂU DÁNG
    text.setFillColor(fillColor.to_sfml_color());
    text.setOutlineColor(strokeColor.to_sfml_color());
    text.setOutlineThickness(strokeWidth * avgScale);

    window.draw(text);
}