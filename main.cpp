#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory> // std::unique_ptr用

// 各図形の設定を格納する構造体
struct ShapeConfig {
    std::string type;    // "rectangle" または "circle"
    std::string name;    // 図形の名前
    sf::Color color;     // 図形の色 (RGB)
    float x, y;          // 初期位置
    float xSpeed, ySpeed;// x方向とy方向の速度
    float width, height; // 長方形の幅と高さ
    float radius;        // 円の半径
};

int main() {
    // 設定ファイルを開く
    std::ifstream configFile("config.txt");
    if (!configFile.is_open()) {
        std::cerr << "config.txtを開けませんでした。\n";
        return -1;
    }

    // 図形設定とウィンドウサイズを読み込む
    int windowWidth = 800, windowHeight = 600;
    std::vector<ShapeConfig> shapes;

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "window") {
            iss >> windowWidth >> windowHeight; // ウィンドウの幅と高さを設定
        } else if (type == "rectangle" || type == "circle") {
            ShapeConfig shape;
            shape.type = type;

            // 名前、位置、速度、色を読み込む
            iss >> shape.name >> shape.x >> shape.y >> shape.xSpeed >> shape.ySpeed;
            int r, g, b;
            iss >> r >> g >> b;
            shape.color = sf::Color(r, g, b);

            // 長方形か円かによって特有のサイズを読み込む
            if (type == "rectangle") {
                iss >> shape.width >> shape.height;
            } else if (type == "circle") {
                iss >> shape.radius;
            }

            shapes.push_back(shape); // 設定をリストに追加
        }
    }

    // フォントを読み込む
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "フォントを読み込めませんでした。\n";
        return -1;
    }

    // ウィンドウを作成
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Collision Detection");
    window.setFramerateLimit(60);

    // 図形とテキストを作成
    std::vector<std::unique_ptr<sf::Shape>> sfmlShapes;
    std::vector<sf::Text> sfmlTexts;

    for (const auto& shape : shapes) {
        sf::Text text;
        text.setFont(font);
        text.setString(shape.name); // 図形の名前をテキストに設定
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(24);

        if (shape.type == "rectangle") {
            auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(shape.width, shape.height));
            rect->setPosition(shape.x, shape.y); // 初期位置を設定
            rect->setFillColor(shape.color);    // 色を設定
            sfmlShapes.push_back(std::move(rect)); // 図形をリストに追加
        } else if (shape.type == "circle") {
            auto circ = std::make_unique<sf::CircleShape>(shape.radius);
            circ->setPosition(shape.x, shape.y);
            circ->setFillColor(shape.color);
            sfmlShapes.push_back(std::move(circ));
        }

        // テキストを図形の中心に配置
        auto bounds = sfmlShapes.back()->getGlobalBounds();
        auto textBounds = text.getLocalBounds();
        float centerX = bounds.left + bounds.width / 2.f - textBounds.width / 2.f;
        float centerY = bounds.top + bounds.height / 2.f - textBounds.height / 2.f - textBounds.top;
        text.setPosition(centerX, centerY);

        sfmlTexts.push_back(text);
    }

    // メインループ
    while (window.isOpen()) {
        // イベント処理
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 図形の更新と衝突処理
        for (size_t i = 0; i < sfmlShapes.size(); ++i) {
            auto& shape = sfmlShapes[i];
            auto& config = shapes[i];

            // 速度に基づいて位置を更新
            sf::Vector2f position = shape->getPosition();
            position.x += config.xSpeed;
            position.y += config.ySpeed;

            // ウィンドウの境界で反射
            if (position.x < 0 || position.x + config.width > window.getSize().x) {
                config.xSpeed *= -1;
            }
            if (position.y < 0 || position.y + config.height > window.getSize().y) {
                config.ySpeed *= -1;
            }
            shape->setPosition(position);

            // 色を元の色にリセット
            shape->setFillColor(config.color);

            // テキスト位置を図形の中心に再設定
            auto bounds = shape->getGlobalBounds();
            auto textBounds = sfmlTexts[i].getLocalBounds();
            float centerX = bounds.left + bounds.width / 2.f - textBounds.width / 2.f;
            float centerY = bounds.top + bounds.height / 2.f - textBounds.height / 2.f - textBounds.top;
            sfmlTexts[i].setPosition(centerX, centerY);

            // 他の図形との衝突をチェック
            for (size_t j = 0; j < sfmlShapes.size(); ++j) {
                if (i == j) continue;

                auto bounds1 = shape->getGlobalBounds();
                auto bounds2 = sfmlShapes[j]->getGlobalBounds();

                // 衝突検知
                if (bounds1.intersects(bounds2)) {
                    config.xSpeed *= -1; // 速度を反転
                    config.ySpeed *= -1;
                    shapes[j].xSpeed *= -1;
                    shapes[j].ySpeed *= -1;

                    // 衝突時に色を一時的に赤に変更
                    shape->setFillColor(sf::Color::Red);
                    sfmlShapes[j]->setFillColor(sf::Color::Red);
                }
            }
        }

        // 描画処理
        window.clear(sf::Color::Black);
        for (const auto& shape : sfmlShapes) {
            window.draw(*shape);
        }
        for (const auto& text : sfmlTexts) {
            window.draw(text);
        }
        window.display();
    }

    return 0;
}
