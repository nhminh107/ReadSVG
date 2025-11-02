#include "ShapeFactory.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;
using namespace rapidxml;

// ====================
//  HÀM CHUYỂN HEX → DEC
// ====================
int ShapeFactory::hexToDec(const std::string& hex)
{
    int value = 0;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> value;
    return value;
}

// ====================
//  HÀM CHUYỂN #RRGGBB → Color
// ====================
Color ShapeFactory::hexToRgba(const std::string& hex_code)
{
    if (hex_code.empty()) return Color(0, 0, 0, 255);
    std::string hex = hex_code;
    if (hex[0] == '#') hex = hex.substr(1);

    if (hex.size() == 6) {
        int r = hexToDec(hex.substr(0, 2));
        int g = hexToDec(hex.substr(2, 2));
        int b = hexToDec(hex.substr(4, 2));
        return Color(r, g, b, 255);
    }
    if (hex.size() == 3) {
        int r = hexToDec(string(2, hex[0]));
        int g = hexToDec(string(2, hex[1]));
        int b = hexToDec(string(2, hex[2]));
        return Color(r, g, b, 255);
    }
    return Color(0, 0, 0, 255);
}

// ====================
//  PHÂN TÍCH CHUỖI MÀU
// ====================
Color ShapeFactory::parseColor(const char* raw_value)
{
    if (!raw_value) return Color(0, 0, 0, 0);

    string val = raw_value;
    val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());

    if (val == "none") return Color(0, 0, 0, 0);
    if (!val.empty() && val[0] == '#') return hexToRgba(val);

    // rgb(r,g,b)
    if (val.rfind("rgb", 0) == 0) {
        size_t start = val.find('(');
        size_t end = val.find(')');
        if (start == string::npos || end == string::npos) return Color(0, 0, 0, 255);

        string inside = val.substr(start + 1, end - start - 1);
        stringstream ss(inside);
        string token;
        vector<float> nums;
        while (getline(ss, token, ',')) nums.push_back(stof(token));

        if (nums.size() >= 3)
            return Color((int)nums[0], (int)nums[1], (int)nums[2], 255);
    }

    // màu tên cơ bản
    if (val == "red") return Color(255, 0, 0, 255);
    if (val == "green") return Color(0, 128, 0, 255);
    if (val == "blue") return Color(0, 0, 255, 255);
    if (val == "yellow") return Color(255, 255, 0, 255);
    if (val == "black") return Color(0, 0, 0, 255);
    if (val == "white") return Color(255, 255, 255, 255);
    if (val == "gray") return Color(128, 128, 128, 255);

    return Color(0, 0, 0, 255);
}

// ====================
//  LẤY GIÁ TRỊ ATTRIBUTE
// ====================
const char* ShapeFactory::getAttr(xml_node<>* node, const char* name)
{
    if (!node) return nullptr;
    auto* attr = node->first_attribute(name);
    return attr ? attr->value() : nullptr;
}

// ====================
//  TẠO SHAPE TỪ NODE XML
// ====================
CShape* ShapeFactory::createShape(xml_node<>* node)
{
    if (!node) return nullptr;
    string tag = node->name();

    Matrix m = Matrix::Identity();

    // --- <circle> ---
    if (tag == "circle") {
        float cx = getAttr(node, "cx") ? stof(getAttr(node, "cx")) : 0.f;
        float cy = getAttr(node, "cy") ? stof(getAttr(node, "cy")) : 0.f;
        float r = getAttr(node, "r") ? stof(getAttr(node, "r")) : 0.f;
        Color fill = parseColor(getAttr(node, "fill"));
        Color stroke = parseColor(getAttr(node, "stroke"));
        float sw = getAttr(node, "stroke-width") ? stof(getAttr(node, "stroke-width")) : 1.f;

        // NOTE: Thêm xử lý fill-opacity & stroke-opacity
        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity"))
            fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity"))
            strokeOpacity = stof(so);

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        return new CCircle(r, cx, cy, fill, stroke, sw, m);
    }

    // --- <rect> ---
    if (tag == "rect") {
        float x = getAttr(node, "x") ? stof(getAttr(node, "x")) : 0.f;
        float y = getAttr(node, "y") ? stof(getAttr(node, "y")) : 0.f;
        float w = getAttr(node, "width") ? stof(getAttr(node, "width")) : 0.f;
        float h = getAttr(node, "height") ? stof(getAttr(node, "height")) : 0.f;
        Color fill = parseColor(getAttr(node, "fill"));
        Color stroke = parseColor(getAttr(node, "stroke"));
        float sw = getAttr(node, "stroke-width") ? stof(getAttr(node, "stroke-width")) : 1.f;

        // NOTE: Thêm xử lý fill-opacity & stroke-opacity
        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity"))
            fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity"))
            strokeOpacity = stof(so);

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        return new CRectangle(w, h, x, y, fill, stroke, m, sw);
    }

    // --- <polygon> ---
    if (tag == "polygon") {
        const char* pts_raw = getAttr(node, "points");
        vector<Point> pts;
        if (pts_raw) {
            stringstream ss(pts_raw);
            string token;
            while (getline(ss, token, ' ')) {
                if (token.empty()) continue;
                size_t comma = token.find(',');
                if (comma != string::npos) {
                    float x = stof(token.substr(0, comma));
                    float y = stof(token.substr(comma + 1));
                    pts.emplace_back(x, y);
                }
            }
        }

        Color fill = parseColor(getAttr(node, "fill"));
        Color stroke = parseColor(getAttr(node, "stroke"));
        float sw = getAttr(node, "stroke-width") ? stof(getAttr(node, "stroke-width")) : 1.f;

        // NOTE: Thêm xử lý fill-opacity & stroke-opacity
        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity"))
            fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity"))
            strokeOpacity = stof(so);

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        return new CPolygon(pts, sw, fill, stroke, m);
    }

    return nullptr; // không nhận dạng được
}
