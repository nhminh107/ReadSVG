#include "CCircle.h"
using namespace std;

void CCircle::draw(sf::RenderWindow& window) const {
    sf::CircleShape shape(radius);

    shape.setPosition(center.xPoint - radius, center.yPoint - radius);

    shape.setFillColor(fillColor.to_sfml_color());
    shape.setOutlineColor(strokeColor.to_sfml_color());
    shape.setOutlineThickness(strokeWidth);

    window.draw(shape);
}