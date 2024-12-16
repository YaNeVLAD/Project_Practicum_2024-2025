#pragma once

#include "SFML/Graphics.hpp"
#include "../View.h"

#include <vector>
#include <memory>

struct KeyBinding
{
	KeyBinding(sf::Keyboard::Key key, const std::function<void()>& onKeyPressed)
		: mKey(key), mOnKeyPressed(onKeyPressed) {}

	sf::Keyboard::Key GetKey() const { return mKey; }

	void Press() { if (mOnKeyPressed) mOnKeyPressed(); }

private:
	sf::Keyboard::Key mKey;
	std::function<void()> mOnKeyPressed = nullptr;
};

class Screen : public sf::Drawable
{
public:
	void AddView(const std::shared_ptr<View>& view) { mViews.push_back(view); }

	void AddKeyBinding(const KeyBinding& binding) {mKeyBindings.push_back(binding); }

	void Clear() { mViews.clear(); }
	void ClearBindings() { mKeyBindings.clear(); }

	void HandleEvents(const sf::RenderWindow& window, const sf::View& camera, const sf::Event& event);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<KeyBinding> mKeyBindings;
	std::vector<std::shared_ptr<View>> mViews;
};