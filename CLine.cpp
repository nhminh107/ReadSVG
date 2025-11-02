#include "CLine.h"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm> // Cần cho std::min

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

    // Độ dài đoạn thẳng (Bán kính)
    float length = startTrans.distance(endTrans);

    // Góc xoay (tính từ trục X dương, cần thiết cho setRotation)
    float angleRadians = atan2(endTrans.yPoint - startTrans.yPoint, endTrans.xPoint - startTrans.xPoint);
    float angleDegrees = angleRadians * 180.0f / M_PI;

    // Tỷ lệ scale: Ước lượng scale factor bằng cách so sánh độ dài gốc và độ dài đã biến đổi.
    float originalLength = m_start.distance(m_end);
    float uniformScale = (originalLength > 0) ? (length / originalLength) : 1.0f;
    float scaledStrokeWidth = strokeWidth * uniformScale;

    // 3. TẠO VÀ CẤU HÌNH SHAPE (Dùng Rectangle để mô phỏng độ dày nét)

    // Chiều rộng là độ dài, Chiều cao là độ dày nét đã scale
    sf::RectangleShape lineShape(sf::Vector2f(length, scaledStrokeWidth));

    // BẮT BUỘC SỬA: Đặt điểm gốc (Origin) về GÓC TRÊN BÊN TRÁI của hình chữ nhật.
    // Điều này đảm bảo lineShape xoay và đặt vị trí từ startTrans.
    // Origin X = 0 (bên trái), Origin Y = scaledStrokeWidth / 2.0f (căn giữa theo chiều cao).
    lineShape.setOrigin(0.0f, scaledStrokeWidth / 2.0f);

    // Đặt vị trí tại điểm bắt đầu (startTrans)
    lineShape.setPosition(startTrans.xPoint, startTrans.yPoint);

    // Xoay hình chữ nhật theo góc đã tính toán
    lineShape.setRotation(angleDegrees);

    // 4. CẤU HÌNH KIỂU DÁNG
    // Màu đường viền (strokeColor) được dùng làm màu nền cho lineShape
    lineShape.setFillColor(strokeColor.to_sfml_color());
    // Độ dày nét viền ngoài của lineShape là 0
    lineShape.setOutlineThickness(0.0f);

    window.draw(lineShape);
}