#include "ProgressBar.h"
#include <algorithm>

bool ProgressBar::Contains(const sf::Vector2f& point) const
{
	return mBackground.getGlobalBounds().contains(point);
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBackground, states);
	target.draw(mProgressLine, states);
}

ProgressBar& ProgressBar::SetSize(sf::Vector2f size)
{
	mBackground.setSize(size);
	mProgressLine.setSize(size);
	return *this;
}

ProgressBar& ProgressBar::SetProgress(float progress)
{
	mProgress = std::clamp(progress, 0.f, 1.f);
	sf::Vector2f backgroundSize = mBackground.getSize();
	mProgressLine.setSize({ backgroundSize.x * mProgress, backgroundSize.y });
	return *this;
}

ProgressBar& ProgressBar::SetBackgroundColor(sf::Color color)
{
	mBackground.setFillColor(color);
	return *this;
}

ProgressBar& ProgressBar::SetProgressLineColor(sf::Color color)
{
	mProgressLine.setFillColor(color);
	return *this;
}

ProgressBar& ProgressBar::SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f offset)
{
	sf::Vector2f position = offset;

	if (alignment == Alignment::CenterX || alignment == Alignment::Center)
	{
		position.x += camera.getCenter().x - mBackground.getSize().x / 2.0f;
	}

	if (alignment == Alignment::CenterY || alignment == Alignment::Center)
	{
		position.y += camera.getCenter().y - mBackground.getSize().y / 2.0f;
	}

	mBackground.setPosition(position);
	mProgressLine.setPosition(position);

	return *this;
}

const sf::Vector2f& ProgressBar::GetPosition()
{
	return mBackground.getPosition();
}
