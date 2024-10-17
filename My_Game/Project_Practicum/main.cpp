#include <SFML/Graphics.hpp>
#include <cmath>
#include "main.h"

const sf::Vector2f eyeRadius = { 60, 100 };
const sf::Vector2f pupilRadius = { 30, 30 };

sf::Vector2f normalizeEllipse(const sf::Vector2f& vector, const sf::Vector2f& radius)
{
	sf::Vector2f normalizedVector = { vector.x / radius.x, vector.y / radius.y };
	float length = std::sqrt(normalizedVector.x * normalizedVector.x + normalizedVector.y * normalizedVector.y);
	if (length != 0)
	{
		return normalizedVector / length;
	}
	return { 0, 0 };
}

void updateEye(const sf::Vector2f& mousePosition, sf::ConvexShape& pupil, const sf::ConvexShape& eye) {
	sf::Vector2f delta = mousePosition - eye.getPosition();

	sf::Vector2f normalizedDelta = normalizeEllipse(delta, eyeRadius);

	sf::Vector2f maxOffset = {
		(eyeRadius.x - pupilRadius.x),
		(eyeRadius.y - pupilRadius.y)
	};

	sf::Vector2f clampedDelta = {
		normalizedDelta.x * maxOffset.x,
		normalizedDelta.y * maxOffset.y
	};

	float deltaLength = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	float clampedLength = std::sqrt(clampedDelta.x * clampedDelta.x + clampedDelta.y * clampedDelta.y);

	if (deltaLength > clampedLength) {
		pupil.setPosition(eye.getPosition() + clampedDelta);
	}
	else {
		pupil.setPosition(eye.getPosition() + delta);
	}
}

// Обработка событий
void pollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition)
{
	sf::Event event{};
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		if (event.type == sf::Event::MouseMoved)
		{
			mousePosition.x = static_cast<float>(event.mouseMove.x);
			mousePosition.y = static_cast<float>(event.mouseMove.y);
		}
	}
}

// Отрисовка кадра
void drawFrame(sf::RenderWindow& window, const sf::ConvexShape& leftEye, const sf::ConvexShape& leftPupil, const sf::ConvexShape& rightEye, const sf::ConvexShape& rightPupil)
{
	window.clear();
	window.draw(leftEye);
	window.draw(leftPupil);
	window.draw(rightEye);
	window.draw(rightPupil);
	window.display();
}

sf::ConvexShape createEllipse(sf::Vector2f pos, sf::Vector2f radius, sf::Color color, int pointCount)
{
	sf::ConvexShape ellipse;
	ellipse.setPointCount(pointCount);

	for (int pointNo = 0; pointNo < pointCount; pointNo++)
	{
		float angle = float(2 * 3.14 * pointNo / pointCount);
		sf::Vector2f point =
		{
			radius.x * std::sin(angle),
			radius.y * std::cos(angle)
		};
		ellipse.setPoint(pointNo, point);
	}

	ellipse.setFillColor(color);
	ellipse.setPosition(pos);

	return ellipse;
}

int main()
{
	constexpr unsigned WINDOW_WIDTH = 800;
	constexpr unsigned WINDOW_HEIGHT = 600;
	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Eyes follow mouse");

	// Инициализация глаз и зрачков
	sf::ConvexShape leftEye = createEllipse({ 300, 300 }, eyeRadius, sf::Color::White, 200);

	sf::ConvexShape leftPupil = createEllipse({ 300, 300 }, pupilRadius, sf::Color::Black, 200);

	sf::ConvexShape rightEye = createEllipse({ 600, 300 }, eyeRadius, sf::Color::White, 200);

	sf::ConvexShape rightPupil = createEllipse({ 600, 300 }, pupilRadius, sf::Color::Black, 200);

	sf::Vector2f mousePosition;

	while (window.isOpen())
	{
		pollEvents(window, mousePosition);
		updateEye(mousePosition, leftPupil, leftEye);
		updateEye(mousePosition, rightPupil, rightEye);
		drawFrame(window, leftEye, leftPupil, rightEye, rightPupil);
	}
}
