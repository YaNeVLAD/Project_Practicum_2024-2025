#include "SFML/Graphics.hpp"
#include <iostream>

constexpr unsigned int FPSLimit = 60;

void setWindowSettings(sf::RenderWindow& window);
sf::CircleShape initPlayer();
void tryMovePlayer(sf::CircleShape& player, sf::Window& window);

float plrX = 100.0, plrY = 100.0, plrRadius = 50.0, plrOutlineThickness = 3.0;

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "My Game");
	setWindowSettings(window);

	sf::Event event;

	sf::CircleShape player = initPlayer();

	std::vector<sf::CircleShape> state;
	state.push_back(player);

	while (window.isOpen())
	{
		//Handle Events
		//-------------
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: window.close(); break;
			case sf::Event::MouseMoved: tryMovePlayer(player, window); break;
			}
		}

		//Draw game
		//-------------
		//Clear game state
		window.clear(sf::Color::Black);
		state.clear();

		//Update objects positions
		//updatePositions(state);

		//Check collisions
		//checkCollisions(state);

		//Load all objects in state
		state.push_back(player);

		//Draw objects
		for (int i = 0; i < state.size(); i++)
		{
			window.draw(state[i]);
		}

		//Render
		window.display();
	}

	return EXIT_SUCCESS;
}

void setWindowSettings(sf::RenderWindow& window)
{
	window.setFramerateLimit(FPSLimit);
}

sf::CircleShape initPlayer()
{
	sf::CircleShape player(plrRadius);
	player.setFillColor(sf::Color::Green);
	player.setPosition(plrX, plrY);
	player.setOrigin(plrRadius / 2, plrRadius / 2);
	player.setOutlineColor(sf::Color::Red);
	player.setOutlineThickness(plrOutlineThickness);

	return player;
}

void tryMovePlayer(sf::CircleShape& player, sf::Window& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		player.setPosition(mousePos.x, mousePos.y);
	}
}

//Игра на C++ и SFML
//SFML - канвас для C++, который позволяет рисовать графику
//Для игры сделать такие же шаги как на практике
//Название, Логлайн, Цель, Препятствие, Жанр, Таблица сравнения, MindMap, RoadMap
//Логлайн - краткое описание сюжета и положения дел в игре
//Таблица сравнения - название игры, что в ней понравилось, насколько понравилось
//MindMap - возможные состояния игры, например, меню/геймплей, у меню есть несколько пунктов и т.д.
//!НЕРЕАЛЬНО! Нарисовать круг