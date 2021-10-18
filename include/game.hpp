#pragma once

#include <SFML/Graphics.hpp>

#include "menu.hpp"
#include "walker.hpp"

enum class GameState
{
    IN_GAME,
    MAIN_MENU,
    EXIT_CONFIRMATION,
};

class Game
{
public:
    Game();
    virtual ~Game() = default;
    void run();

protected:
private:
    Menu m_mainMenu;
    Menu m_exitConfirmationMenu;

    const float m_timeStep = 1.f;
    const float m_timeSlice = 1.f;
    float m_lastTime = 0.f;
    float m_currentSlice = 0.f;

    unsigned int m_windowWidth = 800;
    unsigned int m_windowHeight = 600;
    int m_walkerCount = 15;

    const std::string m_shaderFile = "assets/shaders/light_shader.frag";
    sf::Shader m_lightShader;

    bool m_running = false;
    GameState m_gameState = GameState::MAIN_MENU;

    sf::RenderWindow m_window;
    sf::RenderTexture m_renderTexture;
    sf::Sprite m_spriteWorld;

    std::vector<Walker> m_walkers;

private:
    void initializeWalkers();
    void checkInput();
    void update();
    void draw();
};