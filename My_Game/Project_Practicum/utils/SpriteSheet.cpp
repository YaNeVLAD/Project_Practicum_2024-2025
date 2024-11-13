#include "SpriteSheet.h"

std::vector<sf::Texture> SpriteSheet::LoadTextures(const std::string& filePath, int frameWidth, int frameHeight)
{
	std::vector<sf::Texture> textures;

	sf::Texture texture;
	if (!texture.loadFromFile(filePath))
	{
		std::cerr << "Ошибка при загрузке файла " << filePath << std::endl;
		return textures;
	}

	sf::Vector2u textureSize = texture.getSize();

	int cols = textureSize.x / frameWidth;
	int rows = textureSize.y / frameHeight;

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			sf::IntRect frameRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
			sf::Texture frameTexture;
			frameTexture.loadFromImage(texture.copyToImage(), frameRect);
			textures.push_back(frameTexture);
		}
	}

	return textures;
}