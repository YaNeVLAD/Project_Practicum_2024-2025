#pragma once

#include "SFML/Graphics.hpp"
#include "../View.h"
#include <vector>
#include <memory>

class Screen : public sf::Drawable
{
public:
	void AddView(const std::shared_ptr<View>& view) { mViews.push_back(view); }

	void Clear() { mViews.clear(); }

	void HandleEvents(const sf::RenderWindow& window, const sf::View& camera, const sf::Event& event);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<std::shared_ptr<View>> mViews;
};