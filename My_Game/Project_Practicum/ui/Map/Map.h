#pragma once

#include "../View.h"

class Map : public View
{
public:
	Map() {}

	void Init(const std::string& textureFile);

	void UpdatePosition(const sf::View& camera);

	Map& SetPosition(Alignment alignment, const sf::View& camera, const sf::Vector2f& offset) override;
	Map& SetPosition(Alignment alignment, const View* parent, const sf::Vector2f& offset) override;
	
	sf::Vector2f GetCenter() const override;
	sf::Vector2f GetSize() const override;

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