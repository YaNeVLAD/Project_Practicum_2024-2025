#include "Screen.h"

void Screen::AddView(const std::shared_ptr<View>& view)
{
	mViews.push_back(view);
}

void Screen::AddKeyBinding(const KeyBinding& binding)
{
	mKeyBindings.push_back(binding);
}

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
			if (binding.CheckPressed())
			{
				binding.Press();
			}
		}
		break;

	default:
		break;
	}
}

void Screen::HandleMouseState(const sf::RenderWindow& window, const sf::View& camera)
{
	sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		auto now = std::chrono::steady_clock::now();

		if (mButtonPressTimes.find(sf::Mouse::Left) == mButtonPressTimes.end())
		{
			mButtonPressTimes[sf::Mouse::Left] = now;
		}

		if (now - mButtonPressTimes[sf::Mouse::Left] >= mHoldDelay)
		{
			for (const auto& view : mViews)
			{
				if (view->Contains(worldPos))
				{
					view->Click();
				}
			}
		}
	}
	else
	{
		mButtonPressTimes.erase(sf::Mouse::Left);
	}
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& view : mViews)
	{
		target.draw(*view, states);
	}
}