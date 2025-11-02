#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "rapidxml.hpp"
#include "CShape.h"
#include "CCircle.h"
#include "CRectangle.h"
#include "Polygon.h"
#include "SVGTypes.h"

class ShapeFactory {
private:
    // --- Hỗ trợ nội bộ ---
    static int hexToDec(const std::string& hex);
    static Color hexToRgba(const std::string& hex_code);
    static Color parseColor(const char* raw_value);
    static const char* getAttr(rapidxml::xml_node<>* node, const char* name);

public:
    // --- Hàm Factory chính ---
    static CShape* createShape(rapidxml::xml_node<>* node);
};
