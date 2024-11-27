#include "Button.h"

Button& Button::SetSize(const sf::Vector2f size)
{
	mShape.setSize(size);
	return *this;
}

Button& Button::SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f offset)
{
	sf::Vector2f position = offset;

	if (alignment == Alignment::CenterX || alignment == Alignment::Center)
	{
		position.x += camera.getCenter().x - mShape.getSize().x / 2.0f;
	}

	if (alignment == Alignment::CenterY || alignment == Alignment::Center)
	{
		position.y += camera.getCenter().y - mShape.getSize().y / 2.0f;
	}

	mShape.setPosition(position);

	UpdateTextPosition();

	return *this;
}

Button& Button::SetFillColor(const sf::Color color)
{
	mShape.setFillColor(color);
	return *this;
}

Button& Button::SetText(const std::string& text, const sf::Font& font, unsigned int fontSize, const sf::Color color)
{
	mText.setFont(font);
	mText.setString(text);
	mText.setCharacterSize(fontSize);
	mText.setFillColor(color);

	UpdateTextPosition();

	return *this;
}

bool Button::Contains(const sf::Vector2f& point) const
{
	sf::Vector2f buttonPos = mShape.getPosition();
	sf::Vector2f buttonSize = mShape.getSize();

	return sf::FloatRect(buttonPos, buttonSize).contains(point);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
	target.draw(mText, states);
}

void Button::UpdateTextPosition()
{
	if (!mText.getFont())
	{
		return;
	}

	sf::FloatRect textBounds = mText.getLocalBounds();
	sf::Vector2f buttonPos = mShape.getPosition();
	sf::Vector2f buttonSize = mShape.getSize();

	mText.setPosition(
		buttonPos.x + (buttonSize.x - textBounds.width) / 2.0f - textBounds.left,
		buttonPos.y + (buttonSize.y - textBounds.height) / 2.0f - textBounds.top
	);
}