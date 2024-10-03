#include <cmath>
#include "SFML/Graphics.hpp"
#include "GameConstants.hpp"
#include "Player.h"
#include "lab2.h"

using namespace sf;

constexpr float periodY = 2;
constexpr float amplitudeY = 10;

void setWindowSettings(RenderWindow& window);
//void handleInput(Player& player, Event::KeyEvent event);

int main()
{
	RenderWindow window(VideoMode::getDesktopMode(), "My Game");
	setWindowSettings(window);

	Clock clock;

	Event event;


	Player player(window.getSize().x / 2.f, window.getSize().y / 2.f);
	sf::Vector2f position = player.getPosition();

	float speedY = 0;
	float wavePhase = 0;

	//std::vector<GameObject> state;
	//state.insert(state.end(), player);

	while (window.isOpen())
	{
		//Handle Events
		//-------------
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed: window.close(); break;
			//case Event::KeyPressed: handleInput(player, event.key); break;
			}
		}

		const float deltaTime = clock.restart().asSeconds();

		wavePhase += deltaTime * (2 * 3.14);

		player.setX(player.getX() + player.getSpeed() * deltaTime);
		speedY = amplitudeY * std::sin(wavePhase / periodY);
		player.setY(player.getY() + speedY);

		const Vector2f offset = { player.getX(), player.getY() };

		if ((offset.x + 2 * player.getRadius() >= window.getSize().x) && (player.getSpeed() > 0))
			player.setSpeed(-player.getSpeed());
		if ((offset.x < 0) && (player.getSpeed() < 0))
			player.setSpeed(-player.getSpeed());

		player.setPosition(offset);

		//Draw game
		//-------------
		//Clear game state
		window.clear();
		//state.clear();

		//Update objects positions
		//updatePositions(state);
		//
		//Check collisions
		//checkCollisions(state);
		//
		//Load all objects in state
		//
		//state.push_back(player);
		//
		//Draw objects
		/*for (int i = 0; i < state.size(); i++)
		{
			i.draw(window);
		}*/

		player.draw(window);

		//Render
		window.display();
	}

	return EXIT_SUCCESS;
}

void setWindowSettings(RenderWindow& window)
{
	window.setFramerateLimit(GC::FPS_LIMIT);
}

//void handleInput(Player& player, Event::KeyEvent event)
//{
//	switch (event.scancode)
//	{
//	case Keyboard::Scancode::W: player.move(GC::IDLE, player.getSpeed() * GC::UP); break;
//	case Keyboard::Scancode::A: player.move(player.getSpeed() * GC::LEFT, GC::IDLE); break;
//	case Keyboard::Scancode::S: player.move(GC::IDLE, player.getSpeed() * GC::DOWN); break;
//	case Keyboard::Scancode::D: player.move(player.getSpeed() * GC::RIGHT, GC::IDLE); break;
//	}
//}

//Игра на C++ и SFML
//SFML - канвас для C++, который позволяет рисовать графику
//Для игры сделать такие же шаги как на практике
//Название, Логлайн, Цель, Препятствие, Жанр, Таблица сравнения, MindMap, RoadMap
//Логлайн - краткое описание сюжета и положения дел в игре
//Таблица сравнения - название игры, что в ней понравилось, насколько понравилось
//MindMap - возможные состояния игры, например, меню/геймплей, у меню есть несколько пунктов и т.д.
//!НЕРЕАЛЬНО! Нарисовать круг