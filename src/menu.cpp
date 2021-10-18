#include "menu.hpp"

void Menu::initialize(const float x, const float y, const std::string& title, const MenuItems& menuItems, const std::function<void()> exitCallback)
{
    m_font.loadFromFile(m_fontPath);
    m_exitCallback = exitCallback;

    constexpr auto titleHeight = 100;

    m_titleText.setFont(m_font);
    m_titleText.setString(title);
    m_titleText.setCharacterSize(titleHeight);
    m_titleText.setPosition(x, y);
    m_titleText.setFillColor(sf::Color::White);

    // Menu Items
    auto offset = y + titleHeight + m_padding * 2;

    for (const auto& param : menuItems)
    {
        sf::Text item;
        item.setFont(m_font);
        item.setString(param.first);
        item.setCharacterSize(30);
        item.setPosition(x, offset);
        item.setFillColor(sf::Color::White);

        offset += m_itemHeight + m_padding;

        m_menuItems.push_back(item);
        m_callbacks.push_back(param.second);
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    window.clear();

    window.draw(m_titleText);

    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        auto& item = m_menuItems[i];
        const sf::FloatRect boundingBox = item.getGlobalBounds();
        const sf::FloatRect boundsWithPadding{
            boundingBox.left - m_padding / 2.0f,
            boundingBox.top - m_padding / 2.0f,
            boundingBox.width + m_padding,
            boundingBox.height + m_padding
        };

        // draw selection box
        if (boundsWithPadding.contains(m_mousePos) || m_mouseOverIndex == i)
        {
            m_mouseOverIndex = static_cast<int>(i);

            item.setFillColor(sf::Color::Yellow);

            sf::RectangleShape selector(sf::Vector2f(boundsWithPadding.width, boundsWithPadding.height));
            selector.setPosition(boundsWithPadding.left, boundsWithPadding.top);
            selector.setFillColor(sf::Color(0, 0, 0, 0));
            selector.setOutlineThickness(2);
            selector.setOutlineColor(sf::Color(255, 255, 255, 255));
            window.draw(selector);
        }
        else
        {
            item.setFillColor(sf::Color::White);
        }
        window.draw(item);
    }
    window.display();
}

void Menu::handleInput(const sf::Event& event, const sf::Vector2f& mousePosition)
{
    m_mousePos = mousePosition;

    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
    {
        m_exitCallback();
    }
    else if (((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return)) || ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)))
    {
        m_callbacks[m_mouseOverIndex]();
    }
    //enable menu selection using arrow keys
    else if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S)) && (m_mouseOverIndex < m_menuItems.size() - 1))
    {
        m_mouseOverIndex++;
    }
    else if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W)) && (m_mouseOverIndex > 0))
    {
        m_mouseOverIndex--;
    }
}
