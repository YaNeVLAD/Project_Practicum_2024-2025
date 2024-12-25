#include <iostream>
#include "TextureManager.h"
#include "../../../utils/SpriteSheet.hpp"

TextureManager::TextureMap TextureManager::mTextureCache;
TextureManager::FontMap TextureManager::mFonts;

sf::Font& TextureManager::GetFont(const std::string& fontPath)
{
    if (mFonts.find(fontPath) != mFonts.end())
    {
        return mFonts[fontPath];
    }

    sf::Font font;
    if (!font.loadFromFile(fontPath))
    {
        throw std::exception("Failed to load font");
    }

    mFonts[fontPath] = font;

    return mFonts[fontPath];
}

std::shared_ptr<std::vector<sf::Texture>> TextureManager::GetTextures(const std::string& filePath, int frameWidth, int frameHeight)
{
    auto it = mTextureCache.find(filePath);
    if (it != mTextureCache.end())
    {
        return it->second;
    }

    auto frames = std::make_shared<std::vector<sf::Texture>>(SpriteSheet::LoadTextures(filePath, frameWidth, frameHeight));
    mTextureCache[filePath] = frames;

    return frames;
}