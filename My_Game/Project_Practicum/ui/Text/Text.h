#pragma once
#include "../View.h"

class Text : public View
{
public:
	enum class TextAlignment
	{
		Left,
		Center,
		Right,
	};

	Text() = default;

	Text& SetText(const std::string& text, const sf::Font& font, unsigned int fontSize = 14, const sf::Color color = sf::Color::White);
	Text& SetTextAlignment(TextAlignment alignment);
	Text& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset = { 0.0f, 0.0f }) override;
	Text& SetPosition(Alignment alignment, const View* parent, const sf::Vector2f& offset) override;

	sf::Vector2f GetCenter() const override;
	sf::Vector2f GetSize() const override;

protected:
	bool Contains(const sf::Vector2f& point) const override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Text mText;
	TextAlignment mTextAlignment = TextAlignment::Left;

	void UpdateTextPosition(const sf::Vector2f position);
};