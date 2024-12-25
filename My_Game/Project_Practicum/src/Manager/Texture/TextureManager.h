#pragma once

#include "SFML/Graphics.hpp"
#include <unordered_map>

class TextureManager
{
public:
	using FontMap = std::unordered_map<std::string, sf::Font>;
	using TextureMap = std::unordered_map<std::string, std::shared_ptr<std::vector<sf::Texture>>>;

	static sf::Font& GetFont(const std::string& fontPath);
	static std::shared_ptr<std::vector<sf::Texture>> GetTextures(const std::string& filePath, int frameWidth, int frameHeight);

	static void Clear() 
	{ 
		mFonts.clear();
		mTextureCache.clear();
	}

private:
	static FontMap mFonts;
	static TextureMap mTextureCache;
};