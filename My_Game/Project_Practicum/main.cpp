#include "SFML/Graphics.hpp"
#include "GameConstants.hpp"
#include "Player.h"
#include "lab2.h"
#include "lab3.h"

using namespace sf;

void setWindowSettings(RenderWindow& window);
void handleInput(Player& player, Event::KeyEvent event);

int main()
{
	RenderWindow window(VideoMode::getDesktopMode(), "My Game");
	setWindowSettings(window);

	Event event{};

	Player player(window.getSize().x / 2, window.getSize().y / 2);
	player.initShape();

	//std::vector<GameObject> state;
	//state.insert(state.end(), player);

	//LAB2.1
	RectangleShape background = RectangleShape(Vector2f(150, 330));
	background.setFillColor(Color::White);
	background.setPosition(150, 250);

	CircleShape greenCircle = CircleShape(50);
	greenCircle.setFillColor(Color::Green);
	greenCircle.setPosition(175, 260);

	CircleShape yellowCircle = CircleShape(50);
	yellowCircle.setFillColor(Color::Yellow);
	yellowCircle.setPosition(175, 365);

	CircleShape redCircle = CircleShape(50);
	redCircle.setFillColor(Color::Red);
	redCircle.setPosition(175, 470);

	//LAB2.2
	

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

		//for (int i = 0; i < trafficLights.size(); i++)
		//{
		//	window.draw(trafficLights[i]);
		//}

		window.draw(background);
		window.draw(greenCircle);
		window.draw(yellowCircle);
		window.draw(redCircle);

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

void handleInput(Player& player, Event::KeyEvent event)
{
	switch (event.scancode)
	{
	case Keyboard::Scancode::W: player.move(GC::IDLE, player.getSpeed() * GC::UP); break;
	case Keyboard::Scancode::A: player.move(player.getSpeed() * GC::LEFT, GC::IDLE); break;
	case Keyboard::Scancode::S: player.move(GC::IDLE, player.getSpeed() * GC::DOWN); break;
	case Keyboard::Scancode::D: player.move(player.getSpeed() * GC::RIGHT, GC::IDLE); break;
	}
}

//Игра на C++ и SFML
//SFML - канвас для C++, который позволяет рисовать графику
//Для игры сделать такие же шаги как на практике
//Название, Логлайн, Цель, Препятствие, Жанр, Таблица сравнения, MindMap, RoadMap
//Логлайн - краткое описание сюжета и положения дел в игре
//Таблица сравнения - название игры, что в ней понравилось, насколько понравилось
//MindMap - возможные состояния игры, например, меню/геймплей, у меню есть несколько пунктов и т.д.