#include <sstream>

#include "UI.h"
#include "GameManager.h"
#include <iomanip>

UI::UI(sf::RenderWindow* window, int lives, GameManager* gameManager) 
	: _window(window), _gameManager(gameManager)
{
	for (int i = lives; i > 0; --i)
	{
		sf::CircleShape newLife;
		newLife.setFillColor(sf::Color::Red);	
		newLife.setOutlineColor(sf::Color::Cyan);
		newLife.setOutlineThickness(4.0f);
		newLife.setRadius(LIFE_RADIUS);
		newLife.setPosition((LIFE_RADIUS*2 + LIFE_PADDING) * i, LIFE_PADDING);
		_lives.push_back(newLife);
	}
	_powerupText.setCharacterSize(30);
	_powerupText.setPosition(800, 10);
	_powerupText.setFillColor(sf::Color::Cyan);
	_font.loadFromFile("font/montS.ttf");
	_powerupText.setFont(_font);

	_progressBar = new sf::RectangleShape(sf::Vector2f(100, 20));
	_progressBar->setPosition(_window->getSize().x - 12 - _progressBar->getSize().x, 22);
	_progressBar->setFillColor(sf::Color::Yellow);
	
	_progressBarBackground = new sf::RectangleShape(sf::Vector2f(104, 24));
	_progressBarBackground->setPosition(_window->getSize().x - 10 - _progressBarBackground->getSize().x, 20);
	_progressBarBackground->setFillColor(sf::Color(20,20,100));
}

UI::~UI()
{
}


void UI::updatePowerupText(std::pair<POWERUPS, float> powerup)
{
	//std::ostringstream oss;
	float powerupProgress = powerup.second / POWERUP_TIME;

	switch (powerup.first)
	{
	case bigPaddle:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		_powerupText.setString("big ");
		_powerupText.setFillColor(paddleEffectsColour);
		_progressBar->setFillColor(paddleEffectsColour);
		break;
	case smallPaddle:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		_powerupText.setString("small ");
		_powerupText.setFillColor(paddleEffectsColour);
		_progressBar->setFillColor(paddleEffectsColour);
		break;
	case slowBall:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		_powerupText.setString("slow ");
		_powerupText.setFillColor(ballEffectsColour);
		_progressBar->setFillColor(ballEffectsColour);
		break;
	case fastBall:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		_powerupText.setString("fast ");
		_powerupText.setFillColor(ballEffectsColour);
		_progressBar->setFillColor(ballEffectsColour);
		break;
	case fireBall:
		//oss << std::fixed << std::setprecision(2) << powerup.second;
		_powerupText.setString("fire ");
		_powerupText.setFillColor(extraBallEffectsColour);
		_progressBar->setFillColor(extraBallEffectsColour);
		break;
	case none:
		_powerupText.setString("");
		
		break;
	}
	_progressBar->setScale(sf::Vector2f(powerupProgress, 1.0f));
	
}

void UI::lifeLost(int lives)
{
	_lives[_lives.size() - 1 - lives].setFillColor(sf::Color::Transparent);
}

void UI::render()
{
	_window->draw(_powerupText);
	for (sf::CircleShape life : _lives)
	{
		_window->draw(life);
	}
	_window->draw(*_progressBarBackground);
	_window->draw(*_progressBar);
	
}