#pragma once

#include "../View.h"

class Map : public View
{
public:
	Map() {}

	void Init(const std::string& textureFile);

	void UpdatePosition(const sf::View& camera);

	bool Contains(const sf::Vector2f& point) const override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static const int mVerticalSprites = 3;
	static const int mHorizontalSprites = 3;

	std::string mTextureFile;
	sf::Texture mBgTexture;
	sf::Sprite mBackgrounds[3][3];
	sf::Vector2u mMapSize;
};