#include "SFML/Graphics.hpp"
#include "Player.h"

using namespace sf;

constexpr unsigned int FPS_LIMIT = 60;

enum Direction
{
	UP = -1,
	DOWN = 1,
	LEFT = -1,
	RIGHT = 1,
	IDLE = 0,
};

void setWindowSettings(RenderWindow& window);
void handleInput(Player& player, Event::KeyEvent event);

int main()
{
	RenderWindow window(VideoMode::getDesktopMode(), "My Game");
	setWindowSettings(window);

	Event event;

	Player player(window.getSize().x / 2, window.getSize().y / 2);
	player.initShape();

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
			case Event::KeyPressed: handleInput(player, event.key); break;
			}
		}

		//Draw game
		//-------------
		//Clear game state
		window.clear(Color::Black);
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
	window.setFramerateLimit(FPS_LIMIT);
}

void handleInput(Player& player, Event::KeyEvent event)
{
	switch (event.scancode)
	{
	case Keyboard::Scancode::W: player.move(IDLE, player.getSpeed() * UP); break;
	case Keyboard::Scancode::A: player.move(player.getSpeed() * LEFT, IDLE); break;
	case Keyboard::Scancode::S: player.move(IDLE, player.getSpeed() * DOWN); break;
	case Keyboard::Scancode::D: player.move(player.getSpeed() * RIGHT, IDLE); break;
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