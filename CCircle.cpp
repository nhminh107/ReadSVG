#include "CCircle.h"
using namespace std;

// Hàm này là CÁCH ĐÚNG để xử lý phép biến đổi cho hình tròn/elip trong SFML

void CCircle::draw(sf::RenderWindow& window) const {
    // 1. Sao chép và tính toán các tham số biến đổi
    const Matrix& m = this->getFinalMatrix();

    // Lấy tâm sau biến đổi (Điểm tham chiếu)
    Point centerTrans = center;
    centerTrans.applyTransform(m);

    // Bán kính theo trục X (RX): Biến đổi điểm (cx + r, cy)
    Point right = center; // Sao chép tâm
    right.xPoint += radius; // SỬA: Dùng xPoint
    right.applyTransform(m);
    float scaledRadiusX = right.distance(centerTrans);

    // Bán kính theo trục Y (RY): Biến đổi điểm (cx, cy + r)
    Point top = center; // Sao chép tâm
    top.yPoint += radius; // SỬA: Dùng yPoint
    top.applyTransform(m);
    float scaledRadiusY = top.distance(centerTrans);

    // 2. TẠO SHAPE VÀ ÁP DỤNG BIẾN ĐỔI

    // Khởi tạo shape với bán kính theo trục X (hoặc bán kính gốc)
    // NOTE: Ta dùng scaledRadiusX làm bán kính cơ sở. Nếu scaledRadiusX != scaledRadiusY
    // thì về mặt lý thuyết hình này là elip, nhưng sf::CircleShape sẽ chỉ dùng 1 bán kính.
    // Để có elip đúng, ta có thể dùng bán kính gốc và setScale, nhưng dùng bán kính đã biến đổi
    // là cách tiếp cận phổ biến hơn trong trường hợp này.
    sf::CircleShape shape(scaledRadiusX);

    // Áp dụng TỶ LỆ CO GIÃN THEO TRỤC Y:
    // Tỷ lệ co giãn trục Y so với trục X (tỷ lệ elip)
    // Nếu scaledRadiusX = 0, ta cần xử lý lỗi chia cho 0, nhưng tạm thời bỏ qua vì radius > 0
    if (radius > 0) {
        shape.setScale(1.0f, scaledRadiusY / scaledRadiusX);
    }

    // 3. ĐẶT VỊ TRÍ
    // Góc trên bên trái: centerTrans - scaledRadius (cần tính toán theo kích thước mới)
    // Khi dùng setScale, ta đặt vị trí theo tâm của hình dạng sau khi đã scale.
    shape.setOrigin(radius, radius);
    shape.setPosition(centerTrans.xPoint, centerTrans.yPoint);

    // 4. THIẾT LẬP KIỂU DÁNG
    // Tính trung bình tỷ lệ để scale độ dày nét
    float avgScale = (scaledRadiusX / radius + scaledRadiusY / radius) * 0.5f;

    shape.setOutlineThickness(strokeWidth * avgScale);
    shape.setFillColor(fillColor.to_sfml_color());
    shape.setOutlineColor(strokeColor.to_sfml_color());
    shape.setPointCount(100);

    window.draw(shape);
}

