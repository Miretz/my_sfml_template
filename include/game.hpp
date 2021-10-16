#pragma once

#include <SFML/Graphics.hpp>

#include "walker.hpp"

class Game
{
public:
    Game();
    virtual ~Game() = default;
    void run();

protected:
private:
    const float m_timeStep = 1.f;
    const float m_timeSlice = 1.f;
    float m_lastTime = 0.f;
    float m_currentSlice = 0.f;

    unsigned int m_windowWidth = 1920;
    unsigned int m_windowHeight = 1080;
    int m_walkerCount = 15;

    std::string m_shaderFile = "assets/shaders/light_shader.frag";
    sf::Shader m_lightShader;

    bool m_running = false;

    sf::RenderWindow m_window;
    sf::RenderTexture m_renderTexture;
    sf::Sprite m_spriteWorld;

    std::vector<Walker> m_walkers;
    std::vector<sf::Vertex> m_vertices;

private:
    void initializeWalkers();
    void checkInput();
    void update();
    void draw();
};