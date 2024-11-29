#pragma once

#include "SFML/Graphics.hpp"
#include <unordered_map>

class TextureManager
{
public:
	static sf::Font& GetFont(const std::string& fontPath);
	static std::vector<sf::Texture>& GetTextures(const std::string& filePath, int frameWidth, int frameHeight);

	static void Clear() 
	{ 
		mFonts.clear();
		mTextureCache.clear();
	}

private:
	static std::unordered_map<std::string, sf::Font> mFonts;
	static std::unordered_map<std::string, std::vector<sf::Texture>> mTextureCache;
};