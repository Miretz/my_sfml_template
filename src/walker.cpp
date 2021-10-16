#include "walker.hpp"

int Walker::sSelected = 0;
RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(float mX, float mY)
{
    generateRandomColor();

    m_strenghts[0] = 0.004f;
    m_strenghts[1] = 0.0005f;

    m_shape.setPosition(mX, mY);
    m_shape.setSize({ m_walkerWidth, m_walkerHeight });
    m_shape.setFillColor(sf::Color(static_cast<sf::Uint8>(m_color.x), static_cast<sf::Uint8>(m_color.y), static_cast<sf::Uint8>(m_color.z)));
    m_shape.setOrigin(m_walkerWidth / 2.0f, m_walkerHeight / 2.0f);
}

void Walker::changeStrenght()
{
    sSelected = (sSelected == 0) ? 1 : 0;
}

void Walker::update(float ft, sf::RenderWindow& window)
{
    sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
    sf::Vector2u winSize = window.getSize();

    //follow mouse if it is inside of the window
    if (mousePosition.x > 0.f && mousePosition.y > 0.f &&
        mousePosition.x < winSize.x && mousePosition.y < winSize.y)
    {
        sf::Vector2f direction = mousePosition - m_shape.getPosition();
        normalize(direction);

        direction *= m_strenghts[sSelected];
        direction.x += generateVelocityFloat() * 0.05f;
        direction.y += generateVelocityFloat() * 0.05f;

        m_velocity += direction;

        //clamp velocity
        m_velocity.x = std::min(m_walkerVelocity, m_velocity.x);
        m_velocity.y = std::min(m_walkerVelocity, m_velocity.y);
        m_velocity.x = std::max(-m_walkerVelocity, m_velocity.x);
        m_velocity.y = std::max(-m_walkerVelocity, m_velocity.y);
    }
    else
    {
        if (generateRandomChance())
        {
            m_velocity.x = generateVelocityFloat();
            m_velocity.y = generateVelocityFloat();
        }
    }

    //guard edges of screen
    if ((right() >= winSize.x && m_velocity.x > 0) || (left() <= 0 && m_velocity.x < 0))
    {
        m_velocity.x = 0;
    }
    if ((top() <= 0 && m_velocity.y < 0) || (bottom() >= winSize.y && m_velocity.y > 0))
    {
        m_velocity.y = 0;
    }

    m_shape.move(m_velocity * ft);
}

void Walker::draw(sf::RenderTarget& target)
{
    target.draw(m_shape);
}

sf::Vector2f Walker::getPosition() const noexcept
{
    return m_shape.getPosition();
}

sf::Vector3f Walker::getColor() const noexcept
{
    return m_color;
}

float Walker::left() const noexcept
{
    return m_shape.getPosition().x - m_shape.getSize().x / 2.0f;
}

float Walker::right() const noexcept
{
    return m_shape.getPosition().x + m_shape.getSize().x / 2.0f;
}

float Walker::top() const noexcept
{
    return m_shape.getPosition().y - m_shape.getSize().y / 2.0f;
}

float Walker::bottom() const noexcept
{
    return m_shape.getPosition().y + m_shape.getSize().y / 2.0f;
}

void Walker::generateRandomColor()
{
    float r = gen.randomFloat(0, 255);
    float g = gen.randomFloat(0, 255);
    float b = gen.randomFloat(0, 255);
    m_color = sf::Vector3f(r, g, b);
}

int Walker::generateRandomChance()
{
    return gen.randomChance(0.02f);
}

float Walker::generateVelocityFloat()
{
    return gen.randomFloat(-m_walkerVelocity, m_walkerVelocity);
}

void Walker::normalize(sf::Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
    {
        source.x = source.x / length;
        source.y = source.y / length;
    }
}