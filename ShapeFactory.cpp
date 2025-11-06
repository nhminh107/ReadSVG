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

        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity")) fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity")) strokeOpacity = stof(so);

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        return new CCircle(r, cx, cy, fill, stroke, sw, m);
    }

    // --- <ellipse> ---
    if (tag == "ellipse") {
        float cx = getAttr(node, "cx") ? stof(getAttr(node, "cx")) : 0.f;
        float cy = getAttr(node, "cy") ? stof(getAttr(node, "cy")) : 0.f;
        float rx = getAttr(node, "rx") ? stof(getAttr(node, "rx")) * 2.0f : 0.f; // SVG: bán trục
        float ry = getAttr(node, "ry") ? stof(getAttr(node, "ry")) * 2.0f : 0.f;

        Color fill = parseColor(getAttr(node, "fill"));
        Color stroke = parseColor(getAttr(node, "stroke"));
        float sw = getAttr(node, "stroke-width") ? stof(getAttr(node, "stroke-width")) : 1.f;

        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity")) fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity")) strokeOpacity = stof(so);

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        Point center(cx, cy);
        return new CEllipse(center, rx, ry, fill, stroke, sw, m);
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

        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity")) fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity")) strokeOpacity = stof(so);

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

        float fillOpacity = 1.0f;
        if (const char* fo = getAttr(node, "fill-opacity")) fillOpacity = stof(fo);
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity")) strokeOpacity = stof(so);

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        return new CPolygon(pts, sw, fill, stroke, m);
    }

    // --- <text> ---
    if (tag == "text") {
        // --- Lấy toạ độ (x, y)
        float x = getAttr(node, "x") ? stof(getAttr(node, "x")) : 0.f;
        float y = getAttr(node, "y") ? stof(getAttr(node, "y")) : 0.f;
        Point startPoint(x, y);

        // --- Lấy màu fill / stroke
        Color fill = parseColor(getAttr(node, "fill"));
        Color stroke = parseColor(getAttr(node, "stroke"));
        float sw = getAttr(node, "stroke-width") ? stof(getAttr(node, "stroke-width")) : 0.f;

        // --- Lấy nội dung text
        std::string textStr = node->value() ? node->value() : "";

        // --- Font và kích thước
        std::string fontFamily = getAttr(node, "font-family") ? getAttr(node, "font-family") : "D:/PJ1_OOP/ReadSVG/x64/Debug/times.ttf";
        float fontSize = getAttr(node, "font-size") ? stof(getAttr(node, "font-size")) : 20.f;

        CText* textObj = new CText(
            textStr,
            startPoint,
            fontFamily,
            fontSize,
            fill,
            stroke,
            sw,
            m
        );

        // --- Gọi hàm tải font (nếu có lỗi thì trả nullptr)
        if (!textObj->loadFont()) {
            std::cerr << "⚠️ Lỗi: Không thể tải font '" << fontFamily << "'\n";
        }

        return textObj;
    }

    // --- <line> ---
    if (tag == "line") {
        // Lấy toạ độ điểm đầu và điểm cuối
        float x1 = getAttr(node, "x1") ? stof(getAttr(node, "x1")) : 0.f;
        float y1 = getAttr(node, "y1") ? stof(getAttr(node, "y1")) : 0.f;
        float x2 = getAttr(node, "x2") ? stof(getAttr(node, "x2")) : 0.f;
        float y2 = getAttr(node, "y2") ? stof(getAttr(node, "y2")) : 0.f;

        // Lấy màu stroke
        Color stroke = parseColor(getAttr(node, "stroke"));
        float sw = getAttr(node, "stroke-width") ? stof(getAttr(node, "stroke-width")) : 1.f;

        // Độ trong suốt stroke (nếu có)
        float strokeOpacity = 1.0f;
        if (const char* so = getAttr(node, "stroke-opacity"))
            strokeOpacity = stof(so);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        // Tạo đối tượng CLine
        return new CLine(x1, y1, x2, y2, stroke, sw, m);
    }

    if (tag == "polyline") {
        const char* pts_raw = getAttr(node, "points");
        std::vector<Point> pts;

        // --- SỬA LỖI: Logic phân tích chuỗi tọa độ linh hoạt và an toàn ---
        if (pts_raw) {
            std::string s_pts(pts_raw);
            // THAY THẾ: Dùng std::replace để thay tất cả dấu phẩy bằng khoảng trắng 
            // Giúp stringstream xử lý tất cả các trường hợp (10,20 hoặc 10 20)
            std::replace(s_pts.begin(), s_pts.end(), ',', ' ');

            std::stringstream ss(s_pts);
            float x, y;
            while (ss >> x >> y) { // Đọc lần lượt X và Y
                pts.emplace_back(x, y);
            }
        }

        // --- XỬ LÝ MÀU MẶC ĐỊNH CHUẨN SVG ---
        const char* rawFill = getAttr(node, "fill");
        const char* rawStroke = getAttr(node, "stroke");

        // Mặc định Fill: "none" (Trong suốt hoàn toàn)
        Color fill = parseColor(rawFill ? rawFill : "none");

        // Mặc định Stroke: "black" (Đen mờ đục)
        Color stroke = parseColor(rawStroke ? rawStroke : "black");

        // Độ dày nét mặc định 1.0f
        float sw = getAttr(node, "stroke-width") ? std::stof(getAttr(node, "stroke-width")) : 1.f;

        // --- ÁP DỤNG OPACITY ---
        float fillOpacity = getAttr(node, "fill-opacity") ? std::stof(getAttr(node, "fill-opacity")) : 1.0f;
        float strokeOpacity = getAttr(node, "stroke-opacity") ? std::stof(getAttr(node, "stroke-opacity")) : 1.0f;

        fill.a = static_cast<int>(fill.a * fillOpacity);
        stroke.a = static_cast<int>(stroke.a * strokeOpacity);

        // Polyline là mở (false)
        bool isClosed = false;

        // Constructor CPolygon(pts, sw, fill, stroke, m, isClosed)
        return new CPolygon(pts, sw, fill, stroke, m, isClosed);
    }

    return nullptr; // không nhận dạng được
}