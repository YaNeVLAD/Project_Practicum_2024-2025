#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>

class SpriteSheet
{
public:
	static std::vector<sf::Texture> LoadTextures(
		const std::string& filePath, int frameWidth, int frameHeight
	);
};

