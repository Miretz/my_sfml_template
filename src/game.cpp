#include "game.hpp"

#include <chrono>
#include <memory>
#include <vector>

#include "SFML/Graphics/Shader.hpp"

Game::Game()
{
    window_.create(sf::VideoMode::getDesktopMode(), "Followers!", sf::Style::Fullscreen);
    window_.setFramerateLimit(500);
    window_.setVerticalSyncEnabled(true);

    windowWidth_ = window_.getSize().x;
    windowHeight_ = window_.getSize().y;

    auto goInGame = [&]() { gameState_ = GameState::IN_GAME; };
    auto goMainMenu = [&]() { gameState_ = GameState::MAIN_MENU; };
    auto goExitMenu = [&]() { gameState_ = GameState::EXIT_CONFIRMATION; };
    auto quit = [&]() { running_ = false; };

    mainMenu_.initialize(
        kMenuX,
        kMenuY,
        "My SFML Game",
        {
            { "New Game", goInGame },
            { "Exit", goExitMenu },
        },
        goExitMenu);

    exitConfirmationMenu_.initialize(
        kMenuX,
        kMenuY,
        "Are you sure?",
        {
            { "Yes", quit },
            { "No", goMainMenu },
        },
        goMainMenu);

    // load shader
    lightShader_.loadFromFile(kShaderFile, sf::Shader::Fragment);
    lightShader_.setUniform(
        "frag_ScreenResolution", sf::Vector2f(static_cast<float>(windowWidth_), static_cast<float>(windowHeight_)));

    initializeWalkers();
}

void Game::run()
{
    renderTexture_.create(windowWidth_, windowHeight_);
    spriteWorld_.setTexture(renderTexture_.getTexture());
    spriteWorld_.setOrigin(spriteWorld_.getTextureRect().width / 2.f, spriteWorld_.getTextureRect().height / 2.f);
    spriteWorld_.setPosition(static_cast<float>(windowWidth_) / 2.f, static_cast<float>(windowHeight_) / 2.f);

    running_ = true;

    while (running_)
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

        auto ftSeconds(lastTime_ / 1000.f);
        if (ftSeconds > 0.f)
        {
            window_.setTitle("FT: " + std::to_string(frameTime) + "\tFPS: " + std::to_string(1.f / ftSeconds));
        }
    }
}

void Game::initializeWalkers()
{
    for (int a = 0; a < kWalkerCount; ++a)
    {
        walkers_.emplace_back(window_.getSize().x / 2.f, window_.getSize().y / 2.f);
    }
}

void Game::checkInput()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        const auto &mousePos = (sf::Vector2f)sf::Mouse::getPosition(window_);

        if (gameState_ == GameState::MAIN_MENU)
        {
            mainMenu_.handleInput(event, mousePos);
        }
        else if (gameState_ == GameState::EXIT_CONFIRMATION)
        {
            exitConfirmationMenu_.handleInput(event, mousePos);
        }
        else if (gameState_ == GameState::IN_GAME)
        {
            if (event.type == sf::Event::Closed)
            {
                running_ = false;
            }
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                gameState_ = GameState::MAIN_MENU;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                Walker::changeStrenght();
            }
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
    if (gameState_ == GameState::MAIN_MENU)
    {
        mainMenu_.draw(window_);
        return;
    }
    else if (gameState_ == GameState::EXIT_CONFIRMATION)
    {
        exitConfirmationMenu_.draw(window_);
        return;
    }

    for (auto &walker : walkers_)
    {
        walker.draw(renderTexture_);

        lightShader_.setUniform("frag_LightOrigin", walker.getPosition());
        lightShader_.setUniform("frag_LightColor", walker.getColor());
        lightShader_.setUniform("frag_LightAttenuation", 40.f);

        sf::RenderStates states;
        states.shader = &lightShader_;
        states.blendMode = sf::BlendAdd;

        renderTexture_.draw(spriteWorld_, states);
    }

    renderTexture_.display();
    window_.draw(spriteWorld_);
    window_.display();
}