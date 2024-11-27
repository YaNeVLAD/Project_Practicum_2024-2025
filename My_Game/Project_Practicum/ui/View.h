#pragma once

#include "SFML/Graphics.hpp"
#include <functional>

class View : public sf::Drawable
{
public:
    virtual ~View() = default;

    virtual bool Contains(const sf::Vector2f& point) const = 0;

    void Click() const { if (mOnClick) mOnClick(); }

    void SetOnClickListener(const std::function<void()>& callback) { mOnClick = callback; }

protected:
    std::function<void()> mOnClick = nullptr;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};