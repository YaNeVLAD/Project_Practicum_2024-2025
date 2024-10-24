#include <cmath>
#include "main.h"

#include "SFML/Graphics.hpp"

const float MOVE_SPEED = 20.0f; // px per second
const float ROTATION_SPEED = 90.0f; // deg per second
const int ELLIPSE_POINT_COUNT = 200;
float getAngle(const sf::Vector2f& direction) 
{
	return std::atan2(direction.y, direction.x) * 180 / 3.14f;
}
		vector.x / scale.x,
sf::Vector2f normalize(const sf::Vector2f& vector) 
{
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length != 0) 
	{
		return vector / length;
	}
	return vector;
}
	{
sf::ConvexShape createArrow() 
{
	sf::ConvexShape arrow;
	arrow.setPointCount(7);

	arrow.setPosition(400, 400);
	arrow.setOrigin(300, 350);
		EYE_RADIUS.x - PUPIL_RADIUS.x,
	arrow.setPoint(0, sf::Vector2f(400, 350)); // Кончик стрелки
	arrow.setPoint(1, sf::Vector2f(300, 250)); // Верхний край головы
	arrow.setPoint(2, sf::Vector2f(300, 300)); // Верхняя часть линии стрелки
	arrow.setPoint(3, sf::Vector2f(200, 300)); // Левый край
	arrow.setPoint(4, sf::Vector2f(200, 400)); // Правый край
	arrow.setPoint(5, sf::Vector2f(300, 400)); // Нижняя часть линии стрелки
	arrow.setPoint(6, sf::Vector2f(300, 450)); // Нижний край головы
		normalizedDelta.x * maxPupilOffset.x,
	arrow.setFillColor(sf::Color::Yellow);
	arrow.setOutlineColor(sf::Color::Black);
	arrow.setOutlineThickness(5);
		deltaToMouse.x * deltaToMouse.x + 
	return arrow;
}
	{
		pupil.setPosition(eye.getPosition() + turnedPupilOffset);
	}
	else 
	{
		pupil.setPosition(eye.getPosition() + deltaToMouse);
	}
}

	while (window.isOpen())
	{
		//Handle Events
		//-------------
		while (window.pollEvent(event))
void update(sf::ConvexShape& arrow, sf::Vector2f mousePosition, float deltaTime) 
{
	sf::Vector2f arrowPosition = arrow.getPosition();
	float arrowRotation = arrow.getRotation();

	sf::Vector2f direction = mousePosition - arrowPosition;

	if (std::sqrt(direction.x * direction.x + direction.y * direction.y) > 1.0f) 
	{
		sf::Vector2f directionNorm = normalize(direction);
		arrowPosition += directionNorm * MOVE_SPEED * deltaTime;
	}
{
	float targetAngle = getAngle(direction);
	float angleDifference = targetAngle - arrowRotation;
	window.draw(rightPupil);
	if (angleDifference > 180) angleDifference -= 360;
	if (angleDifference < -180) angleDifference += 360;

	float maxRotation = ROTATION_SPEED * deltaTime;
	if (std::abs(angleDifference) < maxRotation) 
	{
		arrowRotation = targetAngle;
	}
	else 
	{
		arrowRotation += (angleDifference > 0 ? 1 : -1) * maxRotation;
	}
{
	// Обновляем позицию и угол стрелки
	arrow.setPosition(arrowPosition);
	arrow.setRotation(arrowRotation);
}

void draw(sf::RenderWindow& window, sf::ConvexShape arrow) 
{
	window.clear(sf::Color::White);
	window.draw(arrow);
	window.display();
}
		float angle = float(2 * 3.14 * pointNo / pointCount);
int main() 
{
	sf::RenderWindow window(sf::VideoMode(1024, 860), "LAB 6");

	sf::ConvexShape arrow = createArrow();
		};
	sf::Vector2f mousePosition;
	}
	sf::Clock clock;
	ellipse.setFillColor(color);
	while (window.isOpen()) {
		pollEvents(window, mousePosition);
}
		float deltaTime = clock.restart().asSeconds();
{
		update(arrow, mousePosition, deltaTime);

		draw(window, arrow);
	);

	sf::ConvexShape rightEye = createEllipse(
		RIGHT_EYE_CENTER_POS, EYE_RADIUS, sf::Color::White, ELLIPSE_POINT_COUNT
	);
	while (window.isOpen()) {
	sf::ConvexShape rightPupil = createEllipse(
		RIGHT_EYE_CENTER_POS, PUPIL_RADIUS, sf::Color::Black, ELLIPSE_POINT_COUNT
	);

	sf::Vector2f mousePosition;

	while (window.isOpen())
	{
		pollEvents(window, mousePosition);
		updateEye(mousePosition, leftEye, leftPupil);
		updateEye(mousePosition, rightEye, rightPupil);
		drawFrame(window, leftEye, leftPupil, rightEye, rightPupil);

		draw(window, arrow);
	}

//Таблица сравнения - название игры, что в ней понравилось, насколько понравилось
//MindMap - возможные состояния игры, например, меню/геймплей, у меню есть несколько пунктов и т.д.