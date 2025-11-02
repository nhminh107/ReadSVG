#include "CRectangle.h"
using namespace std;

void CRectangle::draw(sf::RenderWindow& window) const {
	Point newPoint = topLeft; //check lại nếu bug 
	newPoint.applyTransform(this->finalMatrix);
	sf::RectangleShape rectangle({width, height});
	rectangle.setPosition(newPoint.xPoint, newPoint.yPoint); 
	rectangle.setFillColor(fillColor.to_sfml_color()); 
	rectangle.setOutlineColor(strokeColor.to_sfml_color()); 
	rectangle.setOutlineThickness(strokeWidth); 
	window.draw(rectangle);
}