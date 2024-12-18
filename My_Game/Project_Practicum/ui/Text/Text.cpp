#include "Text.h"

Text& Text::SetText(const std::string& text, const sf::Font& font, unsigned int fontSize, const sf::Color color)
{
	mText.setFont(font);
	mText.setString(sf::String().fromUtf8(text.begin(), text.end()));
	mText.setCharacterSize(fontSize);
	mText.setFillColor(color);
	
	return *this;
}

Text& Text::SetTextAlignment(TextAlignment alignment)
{
	mTextAlignment = alignment;

	return *this;
}

Text& Text::SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset)
{
	sf::Vector2f position = offset;

	if (alignment == Alignment::CenterX || alignment == Alignment::Center)
	{
		position.x += camera.getCenter().x - mText.getGlobalBounds().width / 2.0f;
	}

	if (alignment == Alignment::CenterY || alignment == Alignment::Center)
	{
		position.y += camera.getCenter().y - mText.getGlobalBounds().height / 2.0f;
	}

	UpdateTextPosition(position);

	return *this;
}

bool Text::Contains(const sf::Vector2f& point) const
{
	return false;
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText);
}

void Text::UpdateTextPosition(const sf::Vector2f position)
{
	sf::FloatRect textBounds = mText.getLocalBounds();
	float offsetX = 0.0f;

	if (mTextAlignment == TextAlignment::Center)
	{
		offsetX = (textBounds.width / 2.0f) - textBounds.left;
	}
	else if (mTextAlignment == TextAlignment::Right)
	{
		offsetX = textBounds.width - textBounds.left;
	}

	mText.setPosition(position.x - offsetX, position.y);
}