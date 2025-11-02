#include "CSVGDocument.h"
#include "ShapeFactory.h"
#include "rapidxml_utils.hpp"
#include <fstream>
#include <sstream>

using namespace std;
using namespace rapidxml;

CSVGDocument::CSVGDocument()
    : scrWidth(0), scrHeight(0), rootMatrix(Matrix::Identity()) {
    scrViewBox = { 0, 0, 0, 0 };
}

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

Matrix CSVGDocument::calculateRootTransform() const {
    if (scrViewBox.widthView == 0 || scrViewBox.heightView == 0)
        return Matrix::Identity();

    float scaleX = scrWidth / scrViewBox.widthView;
    float scaleY = scrHeight / scrViewBox.heightView;
    float uniformScale = std::min(scaleX, scaleY);

    Matrix M_scale = Matrix::createScale(uniformScale, uniformScale);
    Matrix M_translate = Matrix::createTranslation(-scrViewBox.xLeft, -scrViewBox.yLeft);

    return M_scale.multiply(M_translate);
}

void CSVGDocument::applyRootTransform() {
    for (auto* shape : elements) {
        if (!shape) continue;

        Matrix newFinal = rootMatrix.multiply(shape->getFinalMatrix());
        shape->setFinalMatrix(newFinal);
    }
}

bool CSVGDocument::loadFromFile(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Khong the mo file SVG: " << filename << endl;
        return false;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    xml_document<> doc;
    try {
        doc.parse<0>(&content[0]);
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

    if (wAttr && hAttr) {
        scrWidth = stof(wAttr);
        scrHeight = stof(hAttr);
    }
    else {
        scrWidth = 800;
        scrHeight = 800;
    }

    // --- Đọc viewBox ---
    const char* vbAttr = root->first_attribute("viewBox") ? root->first_attribute("viewBox")->value() : nullptr;
    if (vbAttr) {
        stringstream ss(vbAttr);
        ss >> scrViewBox.xLeft >> scrViewBox.yLeft >> scrViewBox.widthView >> scrViewBox.heightView;
    }
    else {
        scrViewBox = { 0, 0, scrWidth, scrHeight };
    }

    // --- Đọc các shape ---
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
    for (auto* e : elements)
        e->draw(window);
}
