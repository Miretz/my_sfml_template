#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "menu.hpp"
#include "walker.hpp"

inline constexpr float kTimeStep = 1.f;
inline constexpr float kTimeSlice = 1.f;
inline constexpr auto kShaderFile = "assets/shaders/light_shader.frag";
inline constexpr float kMenuX = 20.f;
inline constexpr float kMenuY = 20.f;
inline constexpr int kWalkerCount = 15;
inline constexpr unsigned int kDefaultWindowWidth = 800;
inline constexpr unsigned int kDefaultWindowHeight = 600;
inline constexpr auto kGameTitle = "Followers!";
inline constexpr int kFramerateLimit = 500;
inline constexpr auto kLightAttenuation = 40.f;

inline constexpr size_t kMainMenuId = 0;
inline constexpr size_t kOptionsMenuId = 1;
inline constexpr size_t kExitMenuId = 2;

enum class GameState
{
    IN_GAME,
    IN_MENU,
};

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