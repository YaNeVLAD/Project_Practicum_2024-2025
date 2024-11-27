#pragma once

#include "../View.h"

class Button : public View
{
public:
	enum class Alignment
	{
		Default,
		CenterX,
		CenterY,
		Center
	};

	Button() = default;

	Button& SetSize(const sf::Vector2f size);
	Button& SetFillColor(const sf::Color color);
	Button& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f offset = { 0.0f, 0.0f });
	Button& SetText(const std::string& text, const sf::Font& font, unsigned int fontSize = 20, const sf::Color color = sf::Color::White);

protected:
	bool Contains(const sf::Vector2f& point) const override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RectangleShape mShape;
	sf::Text mText;

	void UpdateTextPosition();
};