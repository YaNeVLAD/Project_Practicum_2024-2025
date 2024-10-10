#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

float toDegrees(float radians)
{
    return radians * 180.f / 3.14159265f;
}

void onMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
    mousePosition.x = static_cast<float>(event.x);
    mousePosition.y = static_cast<float>(event.y);
}

void init(sf::ConvexShape& pointer)
{
    pointer.setPointCount(3);
    pointer.setPoint(0, { 40, 0 });
    pointer.setPoint(1, { -40, 25 });
    pointer.setPoint(2, { -40, -25 });
    pointer.setPosition({ 400, 300 });
    pointer.setFillColor(sf::Color(255, 165, 0));
}

void pollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition)
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::MouseMoved)
        {
            onMouseMove(event.mouseMove, mousePosition);
        }
    }
}

void update(const sf::Vector2f& mousePosition, sf::ConvexShape& pointer, const sf::Clock& clock)
{
    sf::Vector2f delta = mousePosition - pointer.getPosition();
    float angle = std::atan2(delta.y, delta.x);
    float currentRotation = pointer.getRotation();
    float targetRotation = toDegrees(angle);

    float maxRotationSpeed = 15;
    float dt = clock.getElapsedTime().asSeconds();
    float maxRotation = maxRotationSpeed * dt;

    float deltaRotation = targetRotation - currentRotation;
    //if (std::abs(deltaRotation) > maxRotation)
    //{
    //    deltaRotation = std::copysign(maxRotation, deltaRotation);
    //}
    pointer.setRotation(currentRotation + deltaRotation);
}

void drawFrame(sf::RenderWindow& window, sf::ConvexShape& pointer)
{
    window.clear();
    window.draw(pointer);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "SFML 3.1");

    sf::ConvexShape pointer;
    init(pointer);
    sf::Vector2f mousePosition;
    sf::Clock clock;

    while (window.isOpen())
    {
        pollEvents(window, mousePosition);
        update(mousePosition, pointer, clock);
        drawFrame(window, pointer);
        clock.restart();
    }
}