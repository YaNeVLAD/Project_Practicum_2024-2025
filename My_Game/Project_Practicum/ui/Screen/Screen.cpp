#include "Screen.h"

void Screen::HandleEvents(const sf::RenderWindow& window, const sf::View& camera, const sf::Event& event)
{
	sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);

	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		for (const auto& view : mViews)
		{
			if (!view->Contains(worldPos))
			{
				continue;
			}
			view->Click();
		}
		break;

	case sf::Event::KeyPressed:
		for (auto& binding : mKeyBindings)
		{
			if (event.key.code != binding.GetKey())
			{
				continue;
			}
			binding.Press();
		}
		break;

	default:
		break;
	}
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& view : mViews)
	{
		target.draw(*view, states);
	}
}
