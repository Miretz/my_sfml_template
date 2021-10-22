#include "walker.hpp"

#include <cmath>

Walker::Walker(float mX, float mY)
{
    generateRandomColor();

    shape_.setPosition(mX, mY);
    shape_.setSize({ kWalkerWidth, kWalkerHeight });
    shape_.setFillColor(
        sf::Color(static_cast<sf::Uint8>(color_.x), static_cast<sf::Uint8>(color_.y), static_cast<sf::Uint8>(color_.z)));
    shape_.setOrigin(kWalkerWidth / kDivideBy, kWalkerHeight / kDivideBy);
}

void Walker::changeStrenght()
{
    Walker::sSelected = (Walker::sSelected == 0) ? 1 : 0;
}

void Walker::update(const float ft, const sf::RenderWindow& window)
{
    const auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    const auto winSize = static_cast<sf::Vector2f>(window.getSize());

    // follow mouse if it is inside of the window
    if (mousePosition.x > 0.f && mousePosition.y > 0.f && mousePosition.x < winSize.x && mousePosition.y < winSize.y)
    {
        sf::Vector2f direction = mousePosition - shape_.getPosition();
        normalize(direction);

        direction *= kStrenghts_[sSelected];  // NOLINT
        direction.x += generateVelocityFloat() * kVelocityModifier;
        direction.y += generateVelocityFloat() * kVelocityModifier;

        velocity_ += direction;

        // clamp velocity
        velocity_.x = std::min(kMaxWalkerVelocity, velocity_.x);
        velocity_.y = std::min(kMaxWalkerVelocity, velocity_.y);
        velocity_.x = std::max(-kMaxWalkerVelocity, velocity_.x);
        velocity_.y = std::max(-kMaxWalkerVelocity, velocity_.y);
    }
    else
    {
        if (generateRandomChance())
        {
            velocity_.x = generateVelocityFloat();
            velocity_.y = generateVelocityFloat();
        }
    }

    // guard edges of screen
    if ((right() >= winSize.x && velocity_.x > 0) || (left() <= 0 && velocity_.x < 0))
    {
        velocity_.x = 0;
    }
    if ((top() <= 0 && velocity_.y < 0) || (bottom() >= winSize.y && velocity_.y > 0))
    {
        velocity_.y = 0;
    }

    shape_.move(velocity_ * ft);
}

void Walker::draw(sf::RenderTarget& target) const noexcept
{
    target.draw(shape_);
}

auto Walker::getPosition() const noexcept -> sf::Vector2f
{
    return shape_.getPosition();
}

auto Walker::getColor() const noexcept -> sf::Vector3f
{
    return color_;
}

auto Walker::left() const noexcept -> float
{
    return shape_.getPosition().x - shape_.getSize().x / kDivideBy;
}

auto Walker::right() const noexcept -> float
{
    return shape_.getPosition().x + shape_.getSize().x / kDivideBy;
}

auto Walker::top() const noexcept -> float
{
    return shape_.getPosition().y - shape_.getSize().y / kDivideBy;
}

auto Walker::bottom() const noexcept -> float
{
    return shape_.getPosition().y + shape_.getSize().y / kDivideBy;
}

void Walker::generateRandomColor()
{
    const auto r = sGen.randomFloat(0, 255);
    const auto g = sGen.randomFloat(0, 255);
    const auto b = sGen.randomFloat(0, 255);
    color_ = sf::Vector3f(r, g, b);
}

auto Walker::generateRandomChance() const noexcept -> int
{
    return sGen.randomChance(kRandomChance);
}

auto Walker::generateVelocityFloat() const noexcept -> float
{
    return sGen.randomFloat(-kMaxWalkerVelocity, kMaxWalkerVelocity);
}

void Walker::normalize(sf::Vector2f& source) const noexcept
{
    const float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0.0f)
    {
        source.x = source.x / length;
        source.y = source.y / length;
    }
}