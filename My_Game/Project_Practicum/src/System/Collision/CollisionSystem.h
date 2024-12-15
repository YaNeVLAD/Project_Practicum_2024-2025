#pragma once

#include "../System.h"

#include <unordered_set>

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
    EntityManager* mEntityManager;
    std::unordered_set<Entity*> mCollidedDamageEntities;

    void DealDamage(Entity* target, DamageComponent* damage);
    void ApplyBonus(Entity* player, Entity* bonus);
	void HandleCollision(Entity* first, Entity* second);
    void ApplyParticleEffect(Entity* player, Entity* particle);
    void HandlePushAway(Entity* first, Entity* second);
};