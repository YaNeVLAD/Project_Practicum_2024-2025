#pragma once

#include "SFML/Graphics.hpp"
#include "View.h"
#include <vector>
#include <memory>

class Layout : public View
{
public:
	Layout() {
		mBackground.setFillColor(sf::Color::Transparent);
	}

	Layout& AddChild(std::shared_ptr<View> child)
	{
		mChildren.push_back(child);
		UpdateBackground();
		return *this;
	}

	Layout& SetSpacing(float spacing)
	{
		mSpacing = spacing;
		UpdateBackground();
		return *this;
	}

	void UpdateBackground()
	{
		mBackground.setSize({ TotalWidth(), TotalHeight() });
	}

	Layout& SetAlignment(Alignment alignment) {
		mAlignment = alignment;
		return *this;
	}

	Layout& SetBackgroundColor(const sf::Color& color) {
		mBackground.setFillColor(color);
		UpdateBackground();
		return *this;
	}

	Layout& UpdateLayout(const sf::View& camera) {
		sf::Vector2f position = offset;

		if (mAlignment == Alignment::CenterX || mAlignment == Alignment::Center)
			position.x += camera.getCenter().x - TotalWidth() / 2.0f;

		if (mAlignment == Alignment::CenterY || mAlignment == Alignment::Center)
			position.y += camera.getCenter().y - TotalHeight() / 2.0f;

		mBackground.setPosition(position);

		for (const auto& child : mChildren) {
			child->SetPosition(mAlignment, camera, position);

			if (mAlignment == Alignment::CenterX || mAlignment == Alignment::Default)
				position.y += child->GetSize().y + mSpacing;
			else
				position.x += child->GetSize().x + mSpacing;
		}

		return *this;
	}

	sf::Vector2f GetSize() const override {
		return mBackground.getSize();
	}

	sf::Vector2f GetCenter() const override {
		return mBackground.getPosition() - mBackground.getSize() / 2.f;
	}

	bool Contains(const sf::Vector2f& point) const override
	{
		for (const auto& child : mChildren)
			if (child->Contains(point))
			{
				child->Click();
				return true;
			}
		return false;
	}

	Layout& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset = { 0,0 }) override {
		sf::Vector2f position = offset;

		if (alignment == Alignment::CenterX || alignment == Alignment::Center)
			position.x += camera.getCenter().x - mBackground.getSize().x / 2.0f;

		if (alignment == Alignment::CenterY || alignment == Alignment::Center)
			position.y += camera.getCenter().y - mBackground.getSize().y / 2.0f;

		mBackground.setPosition(position);
		UpdateLayout(camera);

		return *this;
	}

	Layout& SetPosition(Alignment alignment, const View* parent, const sf::Vector2f& offset = { 0,0 }) override {
		sf::Vector2f position = offset;

		if (alignment == Alignment::CenterX || alignment == Alignment::Center)
			position.x += parent->GetCenter().x - mBackground.getSize().x / 2.0f;

		if (alignment == Alignment::CenterY || alignment == Alignment::Center)
			position.y += parent->GetCenter().y - mBackground.getSize().y / 2.0f;

		mBackground.setPosition(position);
		return *this;
	}

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(mBackground, states);
		for (const auto& child : mChildren)
			target.draw(*child, states);
	}

private:
	std::vector<std::shared_ptr<View>> mChildren;
	sf::RectangleShape mBackground;
	Alignment mAlignment = Alignment::Default;
	float mSpacing = 0;
	sf::Vector2f offset;

	float TotalWidth() const {
		float width = 0;
		for (const auto& child : mChildren) width += child->GetSize().x + mSpacing;
		return mChildren.empty() ? 0 : width - mSpacing;
	}

	float TotalHeight() const {
		float height = 0;
		for (const auto& child : mChildren) height += child->GetSize().y + mSpacing;
		return mChildren.empty() ? 0 : height - mSpacing;
	}
};