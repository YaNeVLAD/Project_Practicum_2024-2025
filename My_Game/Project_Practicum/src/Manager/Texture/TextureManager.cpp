#include <iostream>
#include "TextureManager.h"
#include "../../../utils/SpriteSheet.h"

std::unordered_map<std::string, std::vector<sf::Texture>> TextureManager::mTextureCache;

std::vector<sf::Texture>& TextureManager::GetTextures(const std::string& filePath, int frameWidth, int frameHeight)
{
    if (mTextureCache.find(filePath) != mTextureCache.end())
    {
        return mTextureCache[filePath];
    }

    std::vector<sf::Texture> frames = SpriteSheet::LoadTextures(filePath, frameWidth, frameHeight);
    mTextureCache[filePath] = frames;

    return mTextureCache[filePath];
}