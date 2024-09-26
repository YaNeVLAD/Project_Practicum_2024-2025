#include <iostream>
#include "lab3.h"

std::vector <sf::RectangleShape, sf::CircleShape> Lab3(sf::RenderWindow& window)
{
	std::vector <sf::RectangleShape, sf::CircleShape> figures;

	sf::RectangleShape background = sf::RectangleShape(sf::Vector2f(100, 100));
	background.setFillColor(sf::Color::White);
	background.setPosition(150, 150);

	figures.push_back(background);

	return figures;
}