#include "CSVGDocument.h"
#include "ShapeFactory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Cần cho std::min
#include <stdexcept> // Cần cho std::runtime_error

// RapidXML headers
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp" 

using namespace std;
using namespace rapidxml;

// Giả định: Các hàm setFinalMatrix và các hàm getter của CShape đã có.
// Giả định: Các hàm Matrix::createScale, createTranslation, multiply đã được triển khai.


CSVGDocument::CSVGDocument()
    : scrWidth(0), scrHeight(0), rootMatrix(Matrix::Identity()) {
    scrViewBox = { 0, 0, 0, 0 };
}

// Hàm Hủy (Destructor): BẮT BUỘC để giải phóng bộ nhớ
CSVGDocument::~CSVGDocument() {
    for (auto* e : elements)
        delete e;
}

void CSVGDocument::addElement(CShape* shape) {
    if (shape)
        elements.push_back(shape);
}

const vector<CShape*>& CSVGDocument::getElements() const {
    return elements;
}

void CSVGDocument::setCanvasSize(float w, float h) {
    scrWidth = w;
    scrHeight = h;
}

void CSVGDocument::setViewBox(const viewBox& vb) {
    scrViewBox = vb;
}

void CSVGDocument::setRootMatrix(const Matrix& m) {
    rootMatrix = m;
}

const Matrix& CSVGDocument::getRootMatrix() const {
    return rootMatrix;
}

// -----------------------------------------------------------
// LOGIC MA TRẬN GỐC (ROOT TRANSFORM)
// -----------------------------------------------------------

Matrix CSVGDocument::calculateRootTransform() const {
    if (scrViewBox.widthView == 0 || scrViewBox.heightView == 0)
        return Matrix::Identity();

    // 1. TÍNH TOÁN TỶ LỆ CO GIÃN (SCALING)
    float scaleX = scrWidth / scrViewBox.widthView;
    float scaleY = scrHeight / scrViewBox.heightView;
    float uniformScale = std::min(scaleX, scaleY); // Quy tắc 'meet'

    // 2. TÍNH TOÁN DỊCH CHUYỂN

    // a) Dịch chuyển gốc ViewBox về (0,0) - T_offset
    float translate_origin_x = -scrViewBox.xLeft;
    float translate_origin_y = -scrViewBox.yLeft;

    // b) Dịch chuyển căn giữa (Center Offset)
    float viewbox_width_scaled = scrViewBox.widthView * uniformScale;
    float viewbox_height_scaled = scrViewBox.heightView * uniformScale;
    float offsetX = (scrWidth - viewbox_width_scaled) / 2.0f;
    float offsetY = (scrHeight - viewbox_height_scaled) / 2.0f;

    // 3. TẠO CÁC MA TRẬN
    Matrix M_translate_origin = Matrix::createTranslation(translate_origin_x, translate_origin_y);
    Matrix M_scale = Matrix::createScale(uniformScale, uniformScale);
    Matrix M_offset = Matrix::createTranslation(offsetX, offsetY);

    // 4. KẾT HỢP MA TRẬN (Thứ tự Áp dụng: Translate -> Scale -> Offset)
    // M_Root = M_offset * M_scale * M_translate_origin

    Matrix M_root_scaled_translated = M_scale.multiply(M_translate_origin);
    Matrix M_root = M_offset.multiply(M_root_scaled_translated);

    return M_root;
}

void CSVGDocument::applyRootTransform() {
    // Sử dụng rootMatrix đã được tính toán trong loadFromFile
    const Matrix& root = this->rootMatrix;

    for (CShape* shape : elements) { // Dùng range-based for cho code sạch hơn
        if (!shape) continue;

        // M_new = M_root * M_element (Nhân ma trận gốc vào tất cả các phần tử con)
        Matrix newFinal = root.multiply(shape->getFinalMatrix());
        shape->setFinalMatrix(newFinal);
    }
}

// -----------------------------------------------------------
// LOGIC TẢI FILE VÀ PHÂN TÍCH XML
// -----------------------------------------------------------

bool CSVGDocument::loadFromFile(const std::string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Khong the mo file SVG: " << filename << endl;
        return false;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    std::vector<char> xml_data(content.begin(), content.end());
    xml_data.push_back('\0');

    xml_document<> doc;
    try {
        doc.parse<0>(&xml_data[0]); // Truyền con trỏ an toàn đến vector<char>
    }
    catch (const std::exception& e) {
        cerr << "Loi khi parse SVG: " << e.what() << endl;
        return false;
    }

    xml_node<>* root = doc.first_node("svg");
    if (!root) {
        cerr << "Khong tim thay node <svg>" << endl;
        return false;
    }

    // --- Đọc width / height ---
    const char* wAttr = root->first_attribute("width") ? root->first_attribute("width")->value() : nullptr;
    const char* hAttr = root->first_attribute("height") ? root->first_attribute("height")->value() : nullptr;

    try {
        if (wAttr && hAttr) {
            scrWidth = std::stof(wAttr);
            scrHeight = std::stof(hAttr);
        }
        else {
            scrWidth = 800; // Giá trị mặc định nếu thiếu
            scrHeight = 800;
        }
    }
    catch (const std::exception&) {
        cerr << "Loi chuyen doi kich thuoc. Dung gia tri mac dinh." << endl;
        scrWidth = 800;
        scrHeight = 800;
    }

    // --- Đọc viewBox ---
    const char* vbAttr = root->first_attribute("viewBox") ? root->first_attribute("viewBox")->value() : nullptr;
    if (vbAttr) {
        stringstream ss(vbAttr);
        // SỬA: Đảm bảo đọc 4 giá trị float
        if (!(ss >> scrViewBox.xLeft >> scrViewBox.yLeft >> scrViewBox.widthView >> scrViewBox.heightView)) {
            cerr << "Loi doc viewBox. Dung gia tri mac dinh." << endl;
            scrViewBox = { 0, 0, scrWidth, scrHeight };
        }
    }
    else {
        // Nếu không có viewBox, hệ tọa độ khớp với kích thước cửa sổ
        scrViewBox = { 0, 0, scrWidth, scrHeight };
    }

    // --- Đọc các shape ---
    // NOTE: Cần sử dụng hàm traverseXML đệ quy nếu bạn hỗ trợ thẻ <g>
    for (xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
        CShape* s = ShapeFactory::createShape(node);
        if (s) addElement(s);
    }

    // --- Tính toán & áp dụng transform gốc ---
    rootMatrix = calculateRootTransform();
    applyRootTransform();

    return true;
}

void CSVGDocument::draw(sf::RenderWindow& window) const {
    // NOTE: Cần phải áp dụng rootMatrix vào cửa sổ nếu bạn dùng sf::View 
    // thay vì áp dụng lên từng hình học (nhưng cách hiện tại là hợp lệ)

    for (CShape* e : elements)
        e->draw(window);
}