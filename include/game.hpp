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

constexpr float kTimeStep = 1.f;
constexpr float kTimeSlice = 1.f;
constexpr auto kShaderFile = "assets/shaders/light_shader.frag";
constexpr float kMenuX = 20.f;
constexpr float kMenuY = 20.f;
constexpr int kWalkerCount = 15;

class Game
{
public:
    Game();
    virtual ~Game() = default;
    void run();

protected:
private:
    Menu mainMenu_;
    Menu exitConfirmationMenu_;

    float lastTime_ = 0.f;
    float currentSlice_ = 0.f;

    unsigned int windowWidth_ = 800;
    unsigned int windowHeight_ = 600;

    sf::Shader lightShader_;

    bool running_ = false;
    GameState gameState_ = GameState::MAIN_MENU;

    sf::RenderWindow window_;
    sf::RenderTexture renderTexture_;
    sf::Sprite spriteWorld_;

    std::vector<Walker> walkers_;

private:
    void initializeWalkers();
    void checkInput();
    void update();
    void draw();
};