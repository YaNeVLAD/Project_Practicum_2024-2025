#pragma once

#include "SFML/Graphics.hpp"
#include <unordered_map>

class TextureManager
{
public:
	static std::vector<sf::Texture>& GetTextures(const std::string& filePath, int frameWidth, int frameHeight);

private:
	static std::unordered_map<std::string, std::vector<sf::Texture>> mTextureCache;
};