#pragma once

#include <SFML/Graphics.hpp>

#include "random_generator.hpp"

constexpr float kWalkerWidth = 6.f;
constexpr float kWalkerHeight = 6.f;
constexpr float kMaxWalkerVelocity = 1.f;
constexpr float strenghts_[2] = { 0.004f, 0.0005f };

class Walker
{
public:
    Walker(float mX, float mY);

    static void changeStrenght();
    void update(float ft, sf::RenderWindow &window);
    void draw(sf::RenderTarget &target);
    sf::Vector2f getPosition() const noexcept;
    sf::Vector3f getColor() const noexcept;
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;

private:
    static int sSelected;
    static RandomGenerator sGen;

    sf::RectangleShape shape_;
    sf::Vector2f velocity_;
    sf::Vector3f color_;

    void generateRandomColor();
    int generateRandomChance();
    float generateVelocityFloat();
    void normalize(sf::Vector2f &source);
};
