#include "CText.h"
using namespace std;

void CText::draw(sf::RenderWindow& window) const {
    Point transformedPlace = place;
    transformedPlace.applyTransform(this->getFinalMatrix());

    sf::Font font;
    if (!font.loadFromFile(fontFamily)) return;

    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(fontSize);
    text.setFillColor(fillColor.to_sfml_color());
    text.setPosition(transformedPlace.xPoint, transformedPlace.yPoint);

    //window.draw(text);
}

