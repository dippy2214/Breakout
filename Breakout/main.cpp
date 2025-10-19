#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include <iostream>

int main()
{

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Breakout");
    GameManager gameManager(&window);
    gameManager.initialize();

    sf::Clock clock;
    float deltaTime;

    while (window.isOpen())
    {

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            deltaTime = clock.restart().asSeconds();
            if (gameManager.IsRunning())
            {
                gameManager.update(deltaTime);
            }
            window.clear();
            gameManager.render();
            window.display();
        
    }

    return 0;
}
