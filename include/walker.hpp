#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "random_generator.hpp"

inline constexpr float kWalkerWidth = 6.f;
inline constexpr float kWalkerHeight = 6.f;
inline constexpr float kMaxWalkerVelocity = 1.f;
inline constexpr std::array<float, 2> kStrenghts_{ { 0.004f, 0.0005f } };
inline constexpr auto kRandomChance = 0.002f;
inline constexpr auto kVelocityModifier = 0.05f;
inline constexpr auto kDivideBy = 2.f;

class Walker
{
public:
    Walker(float mX, float mY);

    static void changeStrenght();
    void update(const float ft, const sf::RenderWindow &window);
    void draw(sf::RenderTarget &target) const noexcept;
    auto getPosition() const noexcept -> sf::Vector2f;
    auto getColor() const noexcept -> sf::Vector3f;
    auto left() const noexcept -> float;
    auto right() const noexcept -> float;
    auto top() const noexcept -> float;
    auto bottom() const noexcept -> float;

private:
    inline static int sSelected = 0;                         // NOLINT
    inline static RandomGenerator sGen = RandomGenerator();  // NOLINT

    sf::RectangleShape shape_;
    sf::Vector2f velocity_;
    sf::Vector3f color_;

    void generateRandomColor();
    auto generateRandomChance() const noexcept -> int;
    auto generateVelocityFloat() const noexcept -> float;
    void normalize(sf::Vector2f &source) const noexcept;
};
