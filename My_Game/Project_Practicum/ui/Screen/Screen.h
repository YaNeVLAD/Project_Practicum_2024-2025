#pragma once

#include "SFML/Graphics.hpp"
#include "../View.h"

#include <vector>
#include <memory>

struct KeyBinding
{
	enum PressMode
	{
		AND,
		OR
	};

	KeyBinding(sf::Keyboard::Key key, const std::function<void()>& onKeyPressed)
		: mKeys({ key }), mOnKeyPressed(onKeyPressed), mPressMode(PressMode::OR) {}

	KeyBinding(const std::vector<sf::Keyboard::Key>& keys, PressMode pressMode, const std::function<void()>& onKeyPressed)
		: mKeys(keys), mPressMode(pressMode), mOnKeyPressed(onKeyPressed) {}

	sf::Keyboard::Key GetKey() const { return mKeys.front(); }

	void Press() { if (mOnKeyPressed) mOnKeyPressed(); }

	bool CheckPressed() const
	{
		bool allPressed = true;
		bool anyPressed = false;

		for (auto key : mKeys)
		{
			bool isPressed = sf::Keyboard::isKeyPressed(key);
			if (mPressMode == PressMode::AND)
			{
				allPressed &= isPressed;
			}
			else if (mPressMode == PressMode::OR)
			{
				anyPressed |= isPressed;
			}
		}

		return (mPressMode == PressMode::AND) ? allPressed : anyPressed;
	}

private:
	PressMode mPressMode;
	std::vector<sf::Keyboard::Key> mKeys;
	std::function<void()> mOnKeyPressed = nullptr;
};

class Screen : public sf::Drawable
{
public:
	void AddView(const std::shared_ptr<View>& view);

	void AddKeyBinding(const KeyBinding& binding);

	void Clear() { mViews.clear(); }
	void ClearBindings() { mKeyBindings.clear(); }

	void HandleEvents(const sf::RenderWindow& window, const sf::View& camera, const sf::Event& event);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<KeyBinding> mKeyBindings;
	std::vector<std::shared_ptr<View>> mViews;
};