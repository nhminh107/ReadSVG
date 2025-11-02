#include "Polygon.h"
using namespace std;

void CPolygon::draw(sf::RenderWindow& window) const {
    if (pointList.empty()) return;

    vector<Point> transformed_points = pointList;
    for (Point& p : transformed_points)
        p.applyTransform(finalMatrix);

    // Nếu là polygon (đóng), dùng ConvexShape
    if (isClosed) {
        sf::ConvexShape polygon;
        polygon.setPointCount(transformed_points.size());
        for (size_t i = 0; i < transformed_points.size(); ++i)
            polygon.setPoint(i, sf::Vector2f(transformed_points[i].xPoint, transformed_points[i].yPoint));

        polygon.setFillColor(fillColor.to_sfml_color());
        polygon.setOutlineColor(strokeColor.to_sfml_color());
        polygon.setOutlineThickness(strokeWidth);
        window.draw(polygon);
    }
    // Nếu là polyline (mở), dùng VertexArray để chỉ vẽ đường viền
    else {
        sf::VertexArray polyline(sf::LineStrip, transformed_points.size());
        for (size_t i = 0; i < transformed_points.size(); ++i) {
            polyline[i].position = sf::Vector2f(transformed_points[i].xPoint, transformed_points[i].yPoint);
            polyline[i].color = strokeColor.to_sfml_color();
        }
        window.draw(polyline);
    }
}
