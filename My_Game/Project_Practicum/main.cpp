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
	RectangleShape KLeft = RectangleShape(Vector2f(50, 200));
	KLeft.setFillColor(Color::Blue);
	KLeft.setPosition(400, 200);

	RectangleShape KTop = RectangleShape(Vector2f(50, 150));
	KTop.setFillColor(Color::Blue);
	KTop.setRotation(-135);
	KTop.setPosition(450, 300);

	RectangleShape KBottom = RectangleShape(Vector2f(50, 150));
	KBottom.setFillColor(Color::Blue);
	KBottom.setRotation(-55);
	KBottom.setPosition(430, 320);


	RectangleShape VLeft1 = RectangleShape(Vector2f(50, 200));
	VLeft1.setFillColor(Color::Blue);
	VLeft1.setRotation(-45);
	VLeft1.setPosition(600, 250);

	RectangleShape VRight1 = RectangleShape(Vector2f(50, 200));
	VRight1.setFillColor(Color::Blue);
	VRight1.setRotation(45);
	VRight1.setPosition(850, 218);


	RectangleShape VLeft2 = RectangleShape(Vector2f(50, 200));
	VLeft2.setFillColor(Color::Blue);
	VLeft2.setRotation(-45);
	VLeft2.setPosition(600 + 300, 250);

	RectangleShape VRight2 = RectangleShape(Vector2f(50, 200));
	VRight2.setFillColor(Color::Blue);
	VRight2.setRotation(45);
	VRight2.setPosition(850 + 300, 218);

	//LAB2.3
	ConvexShape roof;
	roof.setFillColor(Color::Red);
	roof.setPosition(1200, 600);
	roof.setPointCount(4);
	roof.setPoint(0, { -90, 0 });
	roof.setPoint(1, { +90, 0 });
	roof.setPoint(2, { +220, 90 });
	roof.setPoint(3, { -220, 90 });

	RectangleShape walls = RectangleShape(Vector2f(350, 300));
	walls.setFillColor(Color(0x44, 0x21, 0x12));
	walls.setPosition(1020, 690);

	RectangleShape door = RectangleShape(Vector2f(100, 150));
	door.setFillColor(Color::Black);
	door.setPosition(1050, 850);

	RectangleShape pipe1 = RectangleShape(Vector2f(50, 80));
	pipe1.setFillColor(Color(0x80, 0x80, 0x80));
	pipe1.setPosition(1230, 570);

	RectangleShape pipe2 = RectangleShape(Vector2f(80, 40));
	pipe2.setFillColor(Color(0x80, 0x80, 0x80));
	pipe2.setPosition(1215, 550);

	CircleShape smoke1 = CircleShape(20);
	smoke1.setFillColor(Color(0xA0,0xA0,0xA0));
	smoke1.setPosition(1250, 520);

	CircleShape smoke2 = CircleShape(25);
	smoke2.setFillColor(Color(0xA0, 0xA0, 0xA0));
	smoke2.setPosition(1265, 495);

	CircleShape smoke3 = CircleShape(30);
	smoke3.setFillColor(Color(0xA0, 0xA0, 0xA0));
	smoke3.setPosition(1275, 460);

	CircleShape smoke4 = CircleShape(35);
	smoke4.setFillColor(Color(0xA0, 0xA0, 0xA0));
	smoke4.setPosition(1275, 420);

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


		window.draw(KLeft);
		window.draw(KTop);
		window.draw(KBottom);
		window.draw(VLeft1);
		window.draw(VRight1);
		window.draw(VLeft2);
		window.draw(VRight2);


		window.draw(roof);
		window.draw(walls);
		window.draw(door);
		window.draw(smoke1);
		window.draw(smoke2);
		window.draw(smoke3);
		window.draw(smoke4);
		window.draw(pipe1);
		window.draw(pipe2);


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