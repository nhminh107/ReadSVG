#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "CShape.h"
#include "SVGTypes.h"
#include "rapidxml.hpp"

struct viewBox {
    float xLeft;
    float yLeft;
    float widthView;
    float heightView;
};

class CSVGDocument {
private:
    std::vector<CShape*> elements;
    float scrWidth;
    float scrHeight;
    viewBox scrViewBox;
    Matrix rootMatrix;

public:
    // Constructor
    CSVGDocument();

    // Destructor
    ~CSVGDocument();

    // Thêm 1 shape vào document
    void addElement(CShape* shape);

    // Trả về danh sách các shape (dùng cho debug / render)
    const std::vector<CShape*>& getElements() const;

    // Thiết lập kích thước canvas
    void setCanvasSize(float w, float h);

    // Thiết lập viewBox
    void setViewBox(const viewBox& vb);

    // Setter cho rootMatrix
    void setRootMatrix(const Matrix& m);

    // Getter
    const Matrix& getRootMatrix() const;

    // Tính toán ma trận gốc từ viewBox + canvas
    Matrix calculateRootTransform() const;

    // Áp dụng ma trận gốc lên toàn bộ elements
    void applyRootTransform();

    // Đọc file SVG, parse các shape và thông tin gốc
    bool loadFromFile(const std::string& filename);

    // Vẽ toàn bộ shapes
    void draw(sf::RenderWindow& window) const;
};
