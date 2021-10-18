#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

using MenuItems = std::vector<std::pair<std::string, std::function<void()>>>;

class Menu
{
public:
    Menu() = default;
    ~Menu() = default;

    void initialize(const float x, const float y, const std::string& title, const MenuItems& menuItems, const std::function<void()> exitCallback);
    void draw(sf::RenderWindow& window);
    void handleInput(const sf::Event& event, const sf::Vector2f& mousePosition);

private:
    sf::Font m_font;
    std::vector<sf::Text> m_menuItems;
    std::vector<std::function<void()>> m_callbacks;
    std::function<void()> m_exitCallback;
    sf::Text m_titleText;
    sf::Vector2f m_mousePos;

    const std::string m_fontPath = "assets/fonts/OtherF.ttf";

    const float m_padding = 20.0f;
    const float m_itemHeight = 30.0f;

    unsigned int m_mouseOverIndex = 0;
};
