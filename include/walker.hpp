#pragma once

#include <SFML/Graphics.hpp>

#include "random_generator.hpp"

class Walker
{
public:
    static int sSelected;
    static RandomGenerator gen;

    Walker(float mX, float mY);
    virtual ~Walker() = default;

    static void changeStrenght();
    void update(float ft, sf::RenderWindow &window);
    void draw(sf::RenderTarget &target);
    sf::Vector2f getPosition() const noexcept;
    sf::Vector3f getColor() const noexcept;
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;

protected:
private:
    float m_walkerWidth = 6.f;
    float m_walkerHeight = 6.f;
    float m_walkerVelocity = 1.f;

    float m_strenghts[2];

    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    sf::Vector3f m_color;

    void generateRandomColor();
    int generateRandomChance();
    float generateVelocityFloat();
    void normalize(sf::Vector2f &source);
};
