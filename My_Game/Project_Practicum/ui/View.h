#pragma once

#include "SFML/Graphics.hpp"
#include <functional>

class View : public sf::Drawable
{
public:
	enum class Alignment
	{
		Default,
		CenterX,
		CenterY,
		Center
	};

	virtual ~View() = default;

	virtual bool Contains(const sf::Vector2f& point) const = 0;

	void Click() const { if (mOnClick) mOnClick(); }

	virtual View& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset = { 0,0 }) = 0;
	virtual View& SetPosition(Alignment alignment, const View* parent, const sf::Vector2f& offset = { 0,0 }) = 0;

	virtual sf::Vector2f GetSize() const = 0;
	virtual sf::Vector2f GetCenter() const = 0;

	void SetOnClickListener(const std::function<void()>& callback) { mOnClick = callback; }

protected:
	std::function<void()> mOnClick = nullptr;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};