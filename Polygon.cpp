#include "Polygon.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

using namespace std;

void CPolygon::draw(sf::RenderWindow& window) const {
    if (pointList.empty()) return;

    // 1️⃣ Lấy ma trận biến đổi cuối cùng
    const Matrix& m = this->getFinalMatrix();

    // 2️⃣ Biến đổi tất cả các điểm
    vector<Point> transformed = pointList;
    for (auto& p : transformed)
        p.applyTransform(m);

    // 3️⃣ Tính scale trung bình từ ma trận (như Circle)
    float scale_x = std::sqrt(m.m[0][0] * m.m[0][0] + m.m[1][0] * m.m[1][0]);
    float scale_y = std::sqrt(m.m[0][1] * m.m[0][1] + m.m[1][1] * m.m[1][1]);
    float avg_scale = (scale_x + scale_y) / 2.0f;

    // 4️⃣ Tạo shape fill
    if (fillColor.a > 0) {
        sf::ConvexShape poly;
        poly.setPointCount(transformed.size());
        for (size_t i = 0; i < transformed.size(); ++i)
            poly.setPoint(i, sf::Vector2f(transformed[i].xPoint, transformed[i].yPoint));

        poly.setFillColor(fillColor.to_sfml_color());
        poly.setOutlineThickness(0.f);
        window.draw(poly);
    }

    // 5️⃣ Tạo stroke (outline)
    if (strokeWidth > 0 && strokeColor.a > 0) {
        sf::ConvexShape outline;
        outline.setPointCount(transformed.size());
        for (size_t i = 0; i < transformed.size(); ++i)
            outline.setPoint(i, sf::Vector2f(transformed[i].xPoint, transformed[i].yPoint));

        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(strokeColor.to_sfml_color());
        outline.setOutlineThickness(strokeWidth * avg_scale); // chỉ scale 1 lần
        window.draw(outline);
    }
}
