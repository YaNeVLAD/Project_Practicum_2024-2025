#include "Screen.h"

void Screen::HandleEvents(const sf::RenderWindow& window, const sf::View& camera, const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);

		for (const auto& view : mViews)
		{
			if (view->Contains(worldPos))
			{
				view->Click();
			}
		}
	}
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& view : mViews)
	{
		target.draw(*view, states);
	}
}
