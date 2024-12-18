#include "Map.h"
#include "../../src/Manager/Texture/TextureManager.h"
#include <iostream>
#include <cmath>

void Map::Init(const std::string& textureFile)
{
	if (!mBgTexture.loadFromFile(textureFile))
	{
		return;
	}

	mMapSize = mBgTexture.getSize();

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			sf::Sprite sprite;
			sprite.setTexture(mBgTexture);
			sprite.setPosition(i * mMapSize.x, j * mMapSize.y);
			mBackgrounds[i][j] = sprite;
		}
	}
}

bool Map::Contains(const sf::Vector2f& point) const
{
	return false;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			target.draw(mBackgrounds[i][j]);
		}
	}
}

void Map::UpdatePosition(const sf::View& camera)
{
	sf::Vector2f viewCenter = camera.getCenter();
	sf::Vector2f viewSize = camera.getSize();

	float offsetX = std::fmod(viewCenter.x, mMapSize.x);
	float offsetY = std::fmod(viewCenter.y, mMapSize.y);

	if (offsetX < 0) offsetX += mMapSize.x;
	if (offsetY < 0) offsetY += mMapSize.y;

	float startX = viewCenter.x - offsetX - mMapSize.x;
	float startY = viewCenter.y - offsetY - mMapSize.y;

	for (int i = 0; i < 3; ++i) 
	{
		for (int j = 0; j < 3; ++j) 
		{
			float xPos = startX + i * mMapSize.x;
			float yPos = startY + j * mMapSize.y;
			mBackgrounds[i][j].setPosition(xPos, yPos);
		}
	}
}