#include "game.hpp"

#include <chrono>
#include <memory>
#include <vector>

#include "SFML/Graphics/Shader.hpp"

void Game::initialize()
{
    initializeWindow();
    initializeMenus();
    initializeWalkers();
    initializeShader();
}

void Game::run()
{
    renderTexture_.create(window_.getSize().x, window_.getSize().y);
    spriteWorld_.setTexture(renderTexture_.getTexture());
    spriteWorld_.setOrigin(static_cast<float>(spriteWorld_.getTextureRect().width) / 2.f, static_cast<float>(spriteWorld_.getTextureRect().height) / 2.f);
    spriteWorld_.setPosition(static_cast<float>(window_.getSize().x) / 2.f, static_cast<float>(window_.getSize().y) / 2.f);

    isRunning_ = true;

    while (isRunning_)
    {
        auto time1(std::chrono::high_resolution_clock::now());

        window_.clear(sf::Color::Black);
        renderTexture_.clear();

        checkInput();
        update();
        draw();

        auto time2(std::chrono::high_resolution_clock::now());
        auto elapsedTime(time2 - time1);
        auto frameTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count();

        lastTime_ = frameTime;

        const auto ftSeconds = lastTime_ / 1000.f;
        if (ftSeconds > 0.f)
        {
            window_.setTitle("FT: " + std::to_string(frameTime) + "\tFPS: " + std::to_string(1.f / ftSeconds));
        }
    }
}

void Game::checkInput()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        const auto &mousePos = (sf::Vector2f)sf::Mouse::getPosition(window_);

        if (gameState_ == GameState::IN_GAME)
        {
            if (event.type == sf::Event::Closed)
            {
                isRunning_ = false;
            }
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                currentMenuId_ = kMainMenuId;
                gameState_ = GameState::IN_MENU;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                Walker::changeStrenght();
            }
        }
        else
        {
            menus_[currentMenuId_].handleInput(event, mousePos);
        }
    }
}

void Game::update()
{
    if (gameState_ != GameState::IN_GAME)
    {
        return;
    }

    currentSlice_ += lastTime_;

    // update multiple times based on fps
    for (; currentSlice_ >= kTimeSlice; currentSlice_ -= kTimeSlice)
    {
        for (auto &walker : walkers_)
        {
            walker.update(kTimeStep, window_);
        }
    }
}

void Game::draw()
{
    if (gameState_ != GameState::IN_GAME)
    {
        menus_[currentMenuId_].draw(window_);
        return;
    }

    for (const auto &walker : walkers_)
    {
        walker.draw(renderTexture_);

        lightShader_.setUniform("frag_LightOrigin", walker.getPosition());
        lightShader_.setUniform("frag_LightColor", walker.getColor());
        lightShader_.setUniform("frag_LightAttenuation", 40.f);

        renderTexture_.draw(spriteWorld_, states_);
    }

    renderTexture_.display();
    window_.draw(spriteWorld_);
    window_.display();
}

void Game::initializeWindow()
{
    if (window_.isOpen())
    {
        window_.close();
    }

    if (isFullscreen_)
    {
        window_.create(sf::VideoMode::getDesktopMode(), kGameTitle, sf::Style::Fullscreen);
    }
    else
    {
        window_.create(sf::VideoMode(kDefaultWindowWidth, kDefaultWindowHeight), kGameTitle, sf::Style::Close);
    }
    window_.setFramerateLimit(kFramerateLimit);
    window_.setVerticalSyncEnabled(true);
}

void Game::initializeShader()
{
    lightShader_.loadFromFile(kShaderFile, sf::Shader::Fragment);
    lightShader_.setUniform(
        "frag_ScreenResolution",
        sf::Vector2f(static_cast<float>(window_.getSize().x), static_cast<float>(window_.getSize().y)));
    states_.shader = &lightShader_;
    states_.blendMode = sf::BlendAdd;
}

void Game::initializeMenus()
{
    menus_.resize(3);
    menus_[kMainMenuId].initialize(
        kMenuX,
        kMenuY,
        "My SFML Game",
        {
            { "New Game", [this]() { gameState_ = GameState::IN_GAME; } },
            { "Options", [this]() { currentMenuId_ = kOptionsMenuId; } },
            { "Exit", [this]() { currentMenuId_ = kExitMenuId; } },
        },
        [this]() { currentMenuId_ = kExitMenuId; });

    menus_[kOptionsMenuId].initialize(
        kMenuX,
        kMenuY,
        "Options",
        {
            { "Toggle Fullscreen",
              [this]()
              {
                  isFullscreen_ = !isFullscreen_;
                  initializeWindow();
              } },
            { "Back", [this]() { currentMenuId_ = kMainMenuId; } },
        },
        [this]() { currentMenuId_ = kMainMenuId; });

    menus_[kExitMenuId].initialize(
        kMenuX,
        kMenuY,
        "Are you sure?",
        {
            { "Yes", [this]() { isRunning_ = false; } },
            { "No", [this]() { currentMenuId_ = kMainMenuId; } },
        },
        [this]() { currentMenuId_ = kMainMenuId; });
}

void Game::initializeWalkers()
{
    for (int a = 0; a < kWalkerCount; ++a)
    {
        walkers_.emplace_back(static_cast<float>(window_.getSize().x) / 2.f, static_cast<float>(window_.getSize().y) / 2.f);
    }
}
