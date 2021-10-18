#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

using MenuItems = std::vector<std::pair<std::string, std::function<void()>>>;

constexpr auto kFontPath = "assets/fonts/OtherF.ttf";
constexpr float kMenuPadding = 20.0f;
constexpr float kMenuItemHeight = 30.0f;
constexpr int kTitleHeight = 100;

class Menu
{
public:
    Menu() = default;
    ~Menu() = default;

    void initialize(
        const float x,
        const float y,
        const std::string& title,
        const MenuItems& menuItems,
        const std::function<void()> exitCallback);
    void draw(sf::RenderWindow& window);
    void handleInput(const sf::Event& event, const sf::Vector2f& mousePosition);

private:
    sf::Font font_;
    std::vector<sf::Text> menuItems_;
    std::vector<std::function<void()>> callbacks_;
    std::function<void()> exitCallback_;
    sf::Text titleText_;
    sf::Vector2f mousePosition_;

    unsigned int selectedIndex_ = 0;
};
