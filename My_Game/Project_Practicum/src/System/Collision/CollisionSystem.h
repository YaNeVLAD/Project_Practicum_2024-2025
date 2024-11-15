#pragma once

#include "../System.h"

struct CollisionEvent {
    float x;
    int type; // 1 - начало, -1 - конец
    Entity* entity;
    sf::FloatRect rect;

    bool operator<(const CollisionEvent& other) const
    {
        if (x != other.x) return x < other.x;
        return type > other.type; // Открытие перед закрытием при одинаковых x
    }

    CollisionEvent(float x, int type, Entity* entity, sf::FloatRect rect)
        : x(x), type(type), entity(entity), rect(rect) {}
};

class CollisionSystem : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	void HandleCollision(Entity* first, Entity* second);
};