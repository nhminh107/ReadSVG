#pragma once
#include "CShape.h"
#include <SFML/Graphics.hpp>
#include <string>

class CText : public CShape {
private:
    std::string content;
    std::string fontFamily;
    float fontSize;
    Point place;
    mutable sf::Font font; //Giữ font sống lâu

public:
    CText(std::string text, std::string font, float size, float x, float y,
        const Color& fColor, const Color& sColor, float sWidth, const Matrix& m)
        : CShape(fColor, sColor, sWidth, m),
        content(std::move(text)), fontFamily(std::move(font)), fontSize(size), place(x, y)
    {
        this->font.loadFromFile(fontFamily);
    }
    void draw(sf::RenderWindow& window) const override;
};
