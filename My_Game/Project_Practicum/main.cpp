#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const float CAT_SPEED = 100.0f; // px per second
const float DELTA_RADIUS = 1.0f; // px

sf::Vector2f normalize(const sf::Vector2f& vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length != 0)
	{
		return vector / length;
	}
	return sf::Vector2f(0, 0);
}

sf::Sprite& createCat(sf::RenderWindow& window)
{
	sf::Texture* catTexture = new sf::Texture();
	catTexture->loadFromFile("resources/cat.png");

	sf::Sprite catSprite{};
	catSprite.setTexture(*catTexture);
	catSprite.setOrigin(
		catSprite.getGlobalBounds().width / 2,
		catSprite.getGlobalBounds().height / 2
	);
	catSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	return catSprite;
}

sf::Sprite& createLaserDot(sf::Sprite catSprite)
{
	sf::Texture* dotTexture = new sf::Texture();
	dotTexture->loadFromFile("resources/red_pointer.png");

	sf::Sprite laserSprite{};
	laserSprite.setTexture(*dotTexture);
	laserSprite.setOrigin(
		laserSprite.getGlobalBounds().width / 2,
		laserSprite.getGlobalBounds().height / 2
	);
	laserSprite.setPosition(catSprite.getPosition());

	return laserSprite;
}

void pollEvents(sf::RenderWindow& window, sf::Sprite& laserSprite)
{
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			laserSprite.setPosition(
				sf::Vector2f(event.mouseButton.x, event.mouseButton.y)
			);
		}
	}
}

void update(sf::Sprite& catSprite, sf::Sprite& laserSprite, float deltaTime, bool& facingRight)
{
	sf::Vector2f direction = laserSprite.getPosition() - catSprite.getPosition();
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance > DELTA_RADIUS) 
	{
		direction = normalize(direction);

		sf::Vector2f distanceToMove = direction * CAT_SPEED * deltaTime;
		catSprite.move(distanceToMove);

		if (laserSprite.getPosition().x > catSprite.getPosition().x && !facingRight) 
		{
			facingRight = true;
			catSprite.setScale(1, 1);
		}
		else if (laserSprite.getPosition().x < catSprite.getPosition().x && facingRight) 
		{
			facingRight = false;
			catSprite.setScale(-1, 1);
		}
	}
}

void draw(sf::RenderWindow& window, sf::Sprite catSprite, sf::Sprite laserSprite)
{
	window.clear(sf::Color::White);
	window.draw(catSprite);
	window.draw(laserSprite);
	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "LAB 6");

	sf::Sprite catSprite = createCat(window);

	sf::Sprite laserSprite = createLaserDot(catSprite);

	sf::Vector2f laserPosition = catSprite.getPosition();

	bool facingRight = true;

	sf::Clock clock;

	while (window.isOpen()) {
		pollEvents(window, laserSprite);

		float deltaTime = clock.restart().asSeconds();

		update(catSprite, laserSprite, deltaTime, facingRight);

		draw(window, catSprite, laserSprite);
	}

	return 0;
}