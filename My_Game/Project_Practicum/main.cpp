#include "SFML/Graphics.hpp"
#include "GameConstants.hpp"

#include "ECS.h"
#include "Components.h"

void setWindowSettings(sf::RenderWindow& window);

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "My Game");
	setWindowSettings(window);

	sf::Event event;

	Manager manager;
	Entity player;
	player.addComponent<Position>();

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: window.close(); break;
			}
		}

		window.clear();



		window.display();
	}

	return EXIT_SUCCESS;
}

void setWindowSettings(sf::RenderWindow& window)
{
	window.setFramerateLimit(GC::FPS_LIMIT);
}