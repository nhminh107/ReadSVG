#pragma once
#include "CShape.h"
#include "SVGTypes.h"
#include <SFML/Graphics.hpp>

class CLine : public CShape {
private:
    Point m_start;
    Point m_end;

public:
    CLine(float x1, float y1, float x2, float y2,
        const Color& sColor, float sWidth, const Matrix& m)
        : CShape(Color(0, 0, 0, 0), sColor, sWidth, m), // Màu fill luôn là Transparent
        m_start(x1, y1),
        m_end(x2, y2)
    {}

    void draw(sf::RenderWindow& window) const override;
};