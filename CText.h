#pragma once
#include <iostream>
#include "SVGTypes.h"
#include "CShape.h"
#include <string>

using namespace std;

class CText : public CShape {
private:
    string content;      
    Point startPoint;     // Tọa độ bắt đầu (x, y)
    string fontFamily;
    float fontSize;

public:
    CText(
        const string& textContent,
        const Point& start,             // Vị trí
        const string& fontFam = "times.ttf",          // Font Family
        float size,                     // Font Size
        const Color& fColor,            // Màu nền (Fill/Text color)
        const Color& sColor,            // Màu viền (Stroke color)
        float sWidth,                   // Độ dày nét viền (Stroke width)
        const Matrix& fMatrix           // Ma trận biến đổi
    ) :
        content(textContent),
        startPoint(start),
        fontFamily(fontFam),
        fontSize(size),
        CShape(fColor, sColor, sWidth, fMatrix)
        // NOTE: Mặc dù <text> không có stroke/strokeWidth, ta vẫn truyền vào để giữ tính nhất quán của CShape.
    {}

    void draw(sf::RenderWindow& window) const override;
};