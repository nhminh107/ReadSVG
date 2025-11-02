#include <SFML/Graphics.hpp>
#include <iostream>
#include "CSVGDocument.h"

int main()
{
    // 1️⃣ Tạo cửa sổSFML
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;  // có thể thử 4, 8 hoặc 16

    sf::RenderWindow window(sf::VideoMode(1200, 600), "SVG Renderer - Factory Pattern", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // 2️⃣ Tạo document SVG
    CSVGDocument svgDoc;

    // 3️⃣ Load file SVG (bạn nhớ đặt test.svg cùng thư mục .exe)
    if (!svgDoc.loadFromFile("SVGImage.XML")) {
        std::cerr << "❌ Khong the load file SVG!" << std::endl;
        return -1;
    }

    std::cout << "✅ SVG loaded successfully!" << std::endl;
    std::cout << "Shapes loaded: " << svgDoc.getElements().size() << std::endl;

    // 4️⃣ Vòng lặp render
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Xóa nền trắng
        window.clear(sf::Color::White);

        // 5️⃣ Vẽ toàn bộ các phần tử trong SVG
        svgDoc.draw(window);

        // Hiển thị lên màn hình
        window.display();
    }

    return 0;
}
