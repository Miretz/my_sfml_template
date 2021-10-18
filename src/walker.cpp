#include "walker.hpp"

#include <cmath>

int Walker::sSelected = 0;
RandomGenerator Walker::sGen = RandomGenerator();

Walker::Walker(float mX, float mY)
{
    generateRandomColor();

    strenghts_[0] = 0.004f;
    strenghts_[1] = 0.0005f;

    shape_.setPosition(mX, mY);
    shape_.setSize({ kWalkerWidth, kWalkerHeight });
    shape_.setFillColor(
        sf::Color(static_cast<sf::Uint8>(color_.x), static_cast<sf::Uint8>(color_.y), static_cast<sf::Uint8>(color_.z)));
    shape_.setOrigin(kWalkerWidth / 2.0f, kWalkerHeight / 2.0f);
}

void Walker::changeStrenght()
{
    sSelected = (sSelected == 0) ? 1 : 0;
}

void Walker::update(float ft, sf::RenderWindow& window)
{
    const sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
    const sf::Vector2u winSize = window.getSize();

    //follow mouse if it is inside of the window
    if (mousePosition.x > 0.f && mousePosition.y > 0.f &&
        mousePosition.x < winSize.x && mousePosition.y < winSize.y)
    {
        sf::Vector2f direction = mousePosition - shape_.getPosition();
        normalize(direction);

        direction *= strenghts_[sSelected];
        direction.x += generateVelocityFloat() * 0.05f;
        direction.y += generateVelocityFloat() * 0.05f;

        velocity_ += direction;

        //clamp velocity
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

    //guard edges of screen
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

void Walker::draw(sf::RenderTarget& target)
{
    target.draw(shape_);
}

sf::Vector2f Walker::getPosition() const noexcept
{
    return shape_.getPosition();
}

sf::Vector3f Walker::getColor() const noexcept
{
    return color_;
}

float Walker::left() const noexcept
{
    return shape_.getPosition().x - shape_.getSize().x / 2.0f;
}

float Walker::right() const noexcept
{
    return shape_.getPosition().x + shape_.getSize().x / 2.0f;
}

float Walker::top() const noexcept
{
    return shape_.getPosition().y - shape_.getSize().y / 2.0f;
}

float Walker::bottom() const noexcept
{
    return shape_.getPosition().y + shape_.getSize().y / 2.0f;
}

void Walker::generateRandomColor()
{
    const auto r = sGen.randomFloat(0, 255);
    const auto g = sGen.randomFloat(0, 255);
    const auto b = sGen.randomFloat(0, 255);
    color_ = sf::Vector3f(r, g, b);
}

int Walker::generateRandomChance()
{
    return sGen.randomChance(0.02f);
}

float Walker::generateVelocityFloat()
{
    return sGen.randomFloat(-kMaxWalkerVelocity, kMaxWalkerVelocity);
}

void Walker::normalize(sf::Vector2f& source)
{
    const float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0.0f)
    {
        source.x = source.x / length;
        source.y = source.y / length;
    }
}