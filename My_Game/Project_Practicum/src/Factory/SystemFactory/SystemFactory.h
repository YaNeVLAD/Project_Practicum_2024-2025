#include "SFML/Graphics.hpp"

class SystemManager;

class SystemFactory
{
public:
	static void InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera);
};
