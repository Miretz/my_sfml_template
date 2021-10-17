#include "game.hpp"

#include <chrono>
#include <memory>
#include <vector>

#include "SFML/Graphics/Shader.hpp"


Game::Game()
{
    m_window.create(sf::VideoMode::getDesktopMode(), "Followers!", sf::Style::Fullscreen);
    m_window.setFramerateLimit(500);
    m_window.setVerticalSyncEnabled(true);

    m_windowWidth = m_window.getSize().x;
    m_windowHeight = m_window.getSize().y;

    //load shader
    m_lightShader.loadFromFile(m_shaderFile, sf::Shader::Fragment);
    m_lightShader.setUniform("frag_ScreenResolution", sf::Vector2f(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)));

    initializeWalkers();
}

void Game::run()
{
    m_renderTexture.create(m_windowWidth, m_windowHeight);
    m_spriteWorld.setTexture(m_renderTexture.getTexture());
    m_spriteWorld.setOrigin(m_spriteWorld.getTextureRect().width / 2.f, m_spriteWorld.getTextureRect().height / 2.f);
    m_spriteWorld.setPosition(static_cast<float>(m_windowWidth) / 2.f, static_cast<float>(m_windowHeight) / 2.f);

    m_running = true;

    while (m_running)
    {
        auto time1(std::chrono::high_resolution_clock::now());

        m_window.clear(sf::Color::Black);
        m_renderTexture.clear();

        checkInput();
        update();
        draw();

        auto time2(std::chrono::high_resolution_clock::now());
        auto elapsedTime(time2 - time1);
        auto frameTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count();

        m_lastTime = frameTime;

        auto ftSeconds(m_lastTime / 1000.f);
        if (ftSeconds > 0.f)
        {
            auto fps(1.f / ftSeconds);
            m_window.setTitle("FT: " + std::to_string(frameTime) + "\tFPS: " + std::to_string(fps));
        }
    }
}

void Game::initializeWalkers()
{
    for (int a = 0; a < m_walkerCount; ++a)
    {
        m_walkers.emplace_back(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
        const auto& color = m_walkers[a].getColor();
        m_vertices.push_back(sf::Vertex(m_walkers[a].getPosition(), sf::Color(sf::Uint8(color.x), sf::Uint8(color.y), sf::Uint8(color.z)), sf::Vector2f(100, 100)));
    }
}

void Game::checkInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }
        else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
        {
            m_running = false;
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            Walker::changeStrenght();
        }
    }
}

void Game::update()
{
    m_currentSlice += m_lastTime;

    //update multiple times based on fps
    for (; m_currentSlice >= m_timeSlice; m_currentSlice -= m_timeSlice)
    {
        for (auto& walker : m_walkers)
        {
            walker.update(m_timeStep, m_window);
        }
    }
}

void Game::draw()
{
    for (int a = 0; a < m_walkerCount; ++a)
    {
        m_vertices[a].position = m_walkers[a].getPosition();

        m_walkers[a].draw(m_renderTexture);

        m_lightShader.setUniform("frag_LightOrigin", m_walkers[a].getPosition());
        m_lightShader.setUniform("frag_LightColor", m_walkers[a].getColor());
        m_lightShader.setUniform("frag_LightAttenuation", 40.f);

        sf::RenderStates states;
        states.shader = &m_lightShader;
        states.blendMode = sf::BlendAdd;

        m_renderTexture.draw(m_spriteWorld, states);
    }

    m_renderTexture.draw(&m_vertices[0], m_vertices.size(), sf::LinesStrip);

    m_renderTexture.display();
    m_window.draw(m_spriteWorld);
    m_window.display();
}