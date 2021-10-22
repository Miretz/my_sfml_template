#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "menu.hpp"
#include "walker.hpp"

enum class GameState
{
    IN_GAME,
    IN_MENU,
};

constexpr float kTimeStep = 1.f;
constexpr float kTimeSlice = 1.f;
constexpr auto kShaderFile = "assets/shaders/light_shader.frag";
constexpr float kMenuX = 20.f;
constexpr float kMenuY = 20.f;
constexpr int kWalkerCount = 15;
constexpr unsigned int kDefaultWindowWidth = 800;
constexpr unsigned int kDefaultWindowHeight = 600;
constexpr auto kGameTitle = "Followers!";
constexpr int kFramerateLimit = 500;
constexpr auto kLightAttenuation = 40.f;

constexpr size_t kMainMenuId = 0;
constexpr size_t kOptionsMenuId = 1;
constexpr size_t kExitMenuId = 2;

class Game
{
public:
    void initialize();
    void run();

private:
    std::vector<Menu> menus_;
    size_t currentMenuId_ = kMainMenuId;

    float lastTime_ = 0.f;
    float currentSlice_ = 0.f;

    sf::Shader lightShader_;

    bool isRunning_ = false;
    bool isFullscreen_ = true;

    GameState gameState_ = GameState::IN_MENU;

    sf::RenderWindow window_;
    sf::RenderStates states_;
    sf::RenderTexture renderTexture_;
    sf::Sprite spriteWorld_;

    std::vector<Walker> walkers_;

private:
    void initializeWindow();
    void initializeMenus();
    void initializeWalkers();
    void initializeShader();
    void checkInput();
    void update();
    void draw();
};