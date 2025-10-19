#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);
    _view = _window->getDefaultView();
    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;
    

    if (_lives <= 0)
    {
        _gameRunning = false;
        _masterText.setString("Game over.");
        SaveScore();
        
        return;
    }
    if (_levelComplete)
    {
        _masterText.setString("Level completed.");
        _gameRunning = false;
        SaveScore();
        return;
    }
    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    if (_shakeTimer > 0)
    {
        if (_view.getTransform() == _window->getDefaultView().getTransform())
        {
            float xoffset = (rand() % 11) - 5;
            float yoffset = (rand() % 11) - 5;
            //std::cout << xoffset << ", " << yoffset << "\n";
            _view.move(xoffset, yoffset);
            _shakeTimer -= dt;
            if (_shakeTimer == 0) _shakeTimer -= 1; //make sure we can keep 0 as a free value for no updates
            _window->setView(_view);
            //std::cout << "random move\n";
        }
        else //reset shake back to 0,0 so that the view doesnt just leave the screen
        {
            _view = _window->getDefaultView();
            _window->setView(_view);
            //std::cout << "reset move\n";
        }
    }
    else if (_shakeTimer < 0)
    {
        _view = _window->getDefaultView();
        _window->setView(_view);
        _shakeTimer = 0;
    }

    // timer.
    _time += dt;


    if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand()%700 == 0)      // TODO parameterise
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    // move paddle
    _paddle->moveRight(dt, _window);
    _paddle->moveLeft(dt, _window);

    // update everything 
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
    _shakeTimer = SCREENSHAKE_TIME_BUFFER;
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

void GameManager::IncrementScore()
{
    _score++;
}

void GameManager::SaveScore()
{
    std::fstream saveFile("scores.txt", std::ios::in | std::ios::out | std::ios::app);
    std::vector<int> scores;
    if (saveFile.is_open())
    {
        int numbuf;

        saveFile << std::to_string(_score) << "\n";
        saveFile.flush(); // make sure it’s written
        saveFile.seekg(0);
        std::string line;
        while (std::getline(saveFile, line))
        {
            std::stringstream ss(line);
            if (ss >> numbuf)
            {
                std::cout << "found a score\n";
                scores.emplace_back(numbuf);
            }
            else
            {
                std::cout << "found a non number\n";
            }
        }
        std::sort(scores.begin(), scores.end(), std::greater<int>());

    }
    else
    {
        std::cout << "failed to open scores file\n";
    }
    saveFile.close();
    size_t count = std::min<size_t>(5, scores.size());
    std::ostringstream oss;
    for (size_t i = 0; i < count; ++i) {
        oss << scores[i];
        if (i != count - 1)
            oss << "\n"; // separate with space
    }

    std::string firstFive = oss.str();
    _masterText.setString(_masterText.getString() + "\n" + firstFive);
}


sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
