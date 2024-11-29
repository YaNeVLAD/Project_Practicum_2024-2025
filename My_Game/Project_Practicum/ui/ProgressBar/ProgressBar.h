#pragma once

#include "../View.h"

class ProgressBar : public View
{
public:
	ProgressBar() = default;

	ProgressBar& SetSize(sf::Vector2f size);
	ProgressBar& SetProgress(float progress);
	ProgressBar& SetBackgroundColor(sf::Color color);
	ProgressBar& SetProgressLineColor(sf::Color color);
	ProgressBar& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f offset = {0.0f, 0.0f});

	const sf::Vector2f& GetPosition();

protected:
	bool Contains(const sf::Vector2f& point) const override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
private:
	float mProgress = 0.f;

	sf::RectangleShape mBackground;
	sf::RectangleShape mProgressLine;
};