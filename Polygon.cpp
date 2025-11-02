#include "Polygon.h"

using namespace std;



void CPolygon::draw(sf::RenderWindow& window) const {

	auto fColor = fillColor.to_sfml_color();

	sf::ConvexShape polygon;


	std::vector<Point> transformed_points = pointList;


	for (Point& p : transformed_points) {

		p.applyTransform(finalMatrix);

	}



	polygon.setPointCount(transformed_points.size());

	for (int i = 0; i < transformed_points.size(); i++) {

		polygon.setPoint(i, sf::Vector2f(transformed_points[i].xPoint, transformed_points[i].yPoint));

	}



	polygon.setFillColor(fillColor.to_sfml_color());

	polygon.setOutlineColor(strokeColor.to_sfml_color());

	polygon.setOutlineThickness(strokeWidth);





	window.draw(polygon);

}