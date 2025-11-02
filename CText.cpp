#include "CText.h"
using namespace std;

void CText::draw(sf::RenderWindow& window) const {
    sf::Text textSF;
    textSF.setFont(font); // dùng font member
    textSF.setString(content);
    textSF.setCharacterSize(static_cast<unsigned int>(fontSize));
    textSF.setFillColor(fillColor.to_sfml_color());
    textSF.setPosition(place.xPoint, place.yPoint);

    window.draw(textSF);
}
