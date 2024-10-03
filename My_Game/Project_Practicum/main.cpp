#include "SFML/Graphics.hpp"
#include "GameConstants.hpp"

using namespace sf;

constexpr int pointCount = 200;
const Vector2f radius = { 200, 50 };

const float PETAL_SIZE = 200;
const float PETAL_COUNT = 6;

const float MOVEMENT_SPEED = 1;
const float MOVEMENT_RADIUS = 350;

const float ROTATION_SPEED = 100;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode::getDesktopMode(), "My Game", Style::Default, settings);

	ConvexShape ellipse;
	ellipse.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
	ellipse.setFillColor(Color(0xFF, 0x60, 0x3D));
	ellipse.setPointCount(pointCount);
	for (int i = 0; i < pointCount; i++)
	{
		float angle = float((2 * 3.14 * i) / pointCount);
		float r = PETAL_SIZE * sin(PETAL_COUNT * angle);
		Vector2f point = {
			r * std::sin(angle),
			r * std::cos(angle),
		};
		ellipse.setPoint(i, point);
	}

	float movementAngle = 0;
	Vector2f movementCenter = { window.getSize().x / 2.f, window.getSize().y / 2.f };

	float rotationAngle = 0;

	Clock clock;
	Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed: window.close(); break;
			}
		}

		window.clear(Color(0x2E, 0x31, 0x36));

		float dt = clock.restart().asSeconds();
		movementAngle += MOVEMENT_SPEED * dt;
		rotationAngle += ROTATION_SPEED * dt;
		float x = movementCenter.x + MOVEMENT_RADIUS * std::cos(movementAngle);
		float y = movementCenter.y + MOVEMENT_RADIUS * std::sin(movementAngle);

		ellipse.setPosition(x, y);
		ellipse.setRotation(rotationAngle);

		window.draw(ellipse);

		window.display();
	}

	return EXIT_SUCCESS;
}