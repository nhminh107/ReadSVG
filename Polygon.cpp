#include "Polygon.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

using namespace std;

void CPolygon::draw(sf::RenderWindow& window) const {
    if (pointList.empty()) return;

    // 1. TẠO BẢN SAO VÀ ÁP DỤNG BIẾN ĐỔI (TRANSFORM)
    std::vector<Point> transformed_points = pointList;

    // Áp dụng phép biến đổi lên từng điểm
    for (Point& p : transformed_points) {
        p.applyTransform(this->finalMatrix);
    }

    // Lấy thuộc tính màu từ lớp cơ sở
    sf::Color fill_color = this->fillColor.to_sfml_color();
    sf::Color stroke_color = this->strokeColor.to_sfml_color();
    float stroke_width = this->strokeWidth;

    // 2. VẼ NỀN (FILL)
    if (fill_color.a > 0) {
        sf::ConvexShape fill_shape;
        fill_shape.setPointCount(transformed_points.size());

        // Gán các điểm đã biến đổi
        for (size_t i = 0; i < transformed_points.size(); ++i) {
            fill_shape.setPoint(i, sf::Vector2f(transformed_points[i].xPoint, transformed_points[i].yPoint));
        }

        fill_shape.setFillColor(fill_color);
        fill_shape.setOutlineThickness(0.0f);

        window.draw(fill_shape);
    }

    // 3. VẼ VIỀN (STROKE)
    if (stroke_width > 0 && stroke_color.a > 0) {
        if (isClosed) {
            // --- VẼ VIỀN CHO POLYGON KÍN ---
            sf::ConvexShape outline_shape;
            outline_shape.setPointCount(transformed_points.size());

            for (size_t i = 0; i < transformed_points.size(); ++i) {
                outline_shape.setPoint(i, sf::Vector2f(transformed_points[i].xPoint, transformed_points[i].yPoint));
            }

            outline_shape.setFillColor(sf::Color::Transparent);
            outline_shape.setOutlineColor(stroke_color);
            outline_shape.setOutlineThickness(stroke_width);

            window.draw(outline_shape);
        }
        else {
            // --- VẼ VIỀN CHO POLYLINE MỞ ---
            // Vẽ các đoạn thẳng nối liền với độ dày tùy chỉnh
            for (size_t i = 0; i < transformed_points.size() - 1; ++i) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(transformed_points[i].xPoint, transformed_points[i].yPoint), stroke_color),
                    sf::Vertex(sf::Vector2f(transformed_points[i + 1].xPoint, transformed_points[i + 1].yPoint), stroke_color)
                };

                // Vẽ hình chữ nhật dày để mô phỏng stroke width
                float dx = transformed_points[i + 1].xPoint - transformed_points[i].xPoint;
                float dy = transformed_points[i + 1].yPoint - transformed_points[i].yPoint;
                float length = std::sqrt(dx * dx + dy * dy);

                if (length > 0) {
                    // Vector vuông góc
                    float perpX = -dy / length * (stroke_width / 2.0f);
                    float perpY = dx / length * (stroke_width / 2.0f);

                    sf::ConvexShape segment(4);
                    segment.setPoint(0, sf::Vector2f(transformed_points[i].xPoint + perpX, transformed_points[i].yPoint + perpY));
                    segment.setPoint(1, sf::Vector2f(transformed_points[i + 1].xPoint + perpX, transformed_points[i + 1].yPoint + perpY));
                    segment.setPoint(2, sf::Vector2f(transformed_points[i + 1].xPoint - perpX, transformed_points[i + 1].yPoint - perpY));
                    segment.setPoint(3, sf::Vector2f(transformed_points[i].xPoint - perpX, transformed_points[i].yPoint - perpY));

                    segment.setFillColor(stroke_color);
                    window.draw(segment);
                }
            }
        }
    }
}