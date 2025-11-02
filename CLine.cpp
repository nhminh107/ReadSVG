#include "CLine.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

using namespace std;

void CLine::draw(sf::RenderWindow& window) const {
    // 1. Sao chép và biến đổi tọa độ
    Point startTrans = m_start;
    Point endTrans = m_end;
    const Matrix& m = this->getFinalMatrix();

    startTrans.applyTransform(m);
    endTrans.applyTransform(m);

    // 2. TÍNH TOÁN HÌNH HỌC CHO SFML

    // Độ dài đoạn thẳng
    float length = startTrans.distance(endTrans);

    // Góc xoay (tính từ trục X dương)
    float angleRadians = atan2(endTrans.yPoint - startTrans.yPoint, endTrans.xPoint - startTrans.xPoint);
    float angleDegrees = angleRadians * 180.0f / M_PI;

    // Tỷ lệ scale (ước lượng)
    float scaleX = startTrans.distance(endTrans) / m_start.distance(m_end);
    float avgScale = std::min(1.0f, scaleX); // Ngăn scale quá lớn

    // 3. TẠO VÀ CẤU HÌNH SHAPE (Dùng Rectangle để mô phỏng độ dày nét)

    // Chiều rộng là độ dài, Chiều cao là độ dày nét
    sf::RectangleShape lineShape(sf::Vector2f(length, strokeWidth));

    // Đặt vị trí tại điểm bắt đầu (x1, y1)
    lineShape.setPosition(startTrans.xPoint, startTrans.yPoint);

    // Xoay hình chữ nhật theo góc đã tính toán
    lineShape.setRotation(angleDegrees);

    // Căn chỉnh màu viền (strokeColor)
    lineShape.setFillColor(strokeColor.to_sfml_color());

    // Tùy chọn: Scale độ dày nét
    // lineShape.setOutlineThickness(strokeWidth * avgScale);

    window.draw(lineShape);
}