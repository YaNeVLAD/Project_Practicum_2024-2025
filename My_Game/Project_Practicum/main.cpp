#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cat");

    sf::Texture catTexture;
    if (!catTexture.loadFromFile("resources/cat.png")) 
    {
        std::cerr << "Ошибка: не удалось загрузить изображение cat.png" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Sprite catSprite;
    catSprite.setTexture(catTexture);

    catSprite.setPosition(
        window.getSize().x / 2.0f - catSprite.getGlobalBounds().width / 2.0f,
        window.getSize().y / 2.0f - catSprite.getGlobalBounds().height / 2.0f
    );

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        window.draw(catSprite);

        window.display();
    }

    return EXIT_SUCCESS;
}