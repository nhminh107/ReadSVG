#pragma once
#include "CShape.h"
#include "SVGTypes.h"
#include <string>
#include <SFML/Graphics.hpp> // Cần thiết cho sf::Font

class CText : public CShape {
private:
    // Thuộc tính riêng của Text
    std::string content;
    Point startPoint;
    std::string fontFamily;
    float fontSize;
    // BỔ SUNG: Font phải được lưu trữ trong lớp để dùng lại (tải 1 lần)
    sf::Font m_font;

public:
    // Constructor
    CText(
        const std::string& textContent,
        const Point& start,
        const std::string& fontFam,
        float size,
        const Color& fColor,
        const Color& sColor,
        float sWidth,
        const Matrix& fMatrix
    ) :
        content(textContent),
        startPoint(start),
        fontFamily(fontFam),
        fontSize(size),
        CShape(fColor, sColor, sWidth, fMatrix)
    {}

    // Khai báo hàm draw() là bắt buộc
    void draw(sf::RenderWindow& window) const override;

    // BỔ SUNG: Hàm tải font một lần duy nhất. Được gọi ngay sau khi tạo đối tượng.
    bool loadFont() {
        return m_font.loadFromFile(fontFamily);
    }
};