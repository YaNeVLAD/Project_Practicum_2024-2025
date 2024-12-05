#pragma once

#include "../System.h"

struct CollisionEvent {
    enum class Type
    {
        Open = 1,
        Close = -1
    };

    float x;
    Type type;
    Entity* entity;
    sf::FloatRect rect;

    bool operator<(const CollisionEvent& other) const
    {
        if (x != other.x) return x < other.x;
        return type > other.type;
    }

    CollisionEvent(float x, Type type, Entity* entity, sf::FloatRect rect)
        : x(x), type(type), entity(entity), rect(rect) {}
};

class CollisionSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
    void DealDamage(Entity* target, DamageComponent* damage);
    void ApplyBonus(EntityManager& em, Entity* player, Entity* bonus);
	void HandleCollision(EntityManager& em, Entity* first, Entity* second);
    void ApplyParticleEffect(Entity* player, Entity* particle);
    void HandlePushAway(Entity* first, Entity* second);
};