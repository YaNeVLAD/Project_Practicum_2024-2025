#include <cmath>
#include "main.h"

#include "SFML/Graphics.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int ELLIPSE_POINT_COUNT = 200;

const sf::Vector2f EYE_RADIUS = { 70, 150 };
const sf::Vector2f PUPIL_RADIUS = { 30, 40 };

const sf::Vector2f LEFT_EYE_CENTER_POS = { 300, 300 };
const sf::Vector2f RIGHT_EYE_CENTER_POS = { 600, 300 };

sf::Vector2f normalize(const sf::Vector2f& vector, const sf::Vector2f& scale)
{
	sf::Vector2f scaledVector = {
		vector.x / scale.x,
		vector.y / scale.y
	};

	float length = std::sqrt(
		scaledVector.x * scaledVector.x + 
		scaledVector.y * scaledVector.y
	);

	if (length != 0)
	{
		return scaledVector / length;
	}
	return { 0, 0 };
}

void updateEye(
	const sf::Vector2f& mousePosition,
	const sf::ConvexShape& eye,
	sf::ConvexShape& pupil
)
{
	sf::Vector2f deltaToMouse = mousePosition - eye.getPosition();

	sf::Vector2f normalizedDelta = normalize(deltaToMouse, EYE_RADIUS);

	sf::Vector2f maxPupilOffset = {
		(EYE_RADIUS.x - PUPIL_RADIUS.x),
		(EYE_RADIUS.y - PUPIL_RADIUS.y)
	};

	sf::Vector2f allowedPupilArea = {
		normalizedDelta.x * maxPupilOffset.x,
		normalizedDelta.y * maxPupilOffset.y
	};

	float deltaToMouseLength = std::sqrt(
		deltaToMouse.x * deltaToMouse.x + 
		deltaToMouse.y * deltaToMouse.y
	);

	float allowedAreaLength = std::sqrt(
		allowedPupilArea.x * allowedPupilArea.x + 
		allowedPupilArea.y * allowedPupilArea.y
	);

	if (deltaToMouseLength > allowedAreaLength) 
	{
		pupil.setPosition(eye.getPosition() + allowedPupilArea);
	}
	else 
	{
		pupil.setPosition(eye.getPosition() + deltaToMouse);
	}
}

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

void drawFrame(
	sf::RenderWindow& window,
	const sf::ConvexShape& leftEye,
	const sf::ConvexShape& leftPupil,
	const sf::ConvexShape& rightEye,
	const sf::ConvexShape& rightPupil
)
{
	window.clear();
	window.draw(leftEye);
	window.draw(leftPupil);
	window.draw(rightEye);
	window.draw(rightPupil);
	window.display();
}

sf::ConvexShape createEllipse(
	sf::Vector2f pos,
	sf::Vector2f radius,
	sf::Color color,
	int pointCount
)
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
	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Eyes follow mouse");

	sf::ConvexShape leftEye = createEllipse(
		LEFT_EYE_CENTER_POS, EYE_RADIUS, sf::Color::White, ELLIPSE_POINT_COUNT
	);

	sf::ConvexShape leftPupil = createEllipse(
		LEFT_EYE_CENTER_POS, PUPIL_RADIUS, sf::Color::Black, ELLIPSE_POINT_COUNT
	);

	sf::ConvexShape rightEye = createEllipse(
		RIGHT_EYE_CENTER_POS, EYE_RADIUS, sf::Color::White, ELLIPSE_POINT_COUNT
	);

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
	}
}
