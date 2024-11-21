#include "CollisionSystem.h"

#include <iostream>
#include <set>

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto entities = entityManager.GetEntitiesWithComponents<CollisionComponent>();
	if (entities.empty()) return;

	std::vector<CollisionEvent> events;

	for (auto& entity : entities)
	{
		auto collision = entity->GetComponent<CollisionComponent>();
		sf::FloatRect rect = collision->getRect();

		events.push_back(CollisionEvent(rect.left, 1, entity, rect));
		events.push_back(CollisionEvent(rect.left + rect.width, -1, entity, rect));
	}

	std::sort(events.begin(), events.end());

	std::set<std::pair<float, Entity*>> active;

	for (const auto& event : events)
	{
		if (event.type == 1)
		{
			for (const auto& activeRect : active)
			{
				const auto& otherEntity = activeRect.second;
				auto otherCollision = otherEntity->GetComponent<CollisionComponent>();
				sf::FloatRect otherRect = otherCollision->getRect();

				if (event.rect.intersects(otherRect))
				{
					HandleCollision(event.entity, otherEntity);
				}
			}
			active.insert(std::pair(event.rect.top, event.entity));
		}
		else
		{
			active.erase(std::pair(event.rect.top, event.entity));
		}
	}
}

void CollisionSystem::ApplyDamage(Entity* entity, int damage)
{
	auto health = entity->GetComponent<HealthComponent>();
	auto playerHealth = entity->GetComponent<PlayerHealthComponent>();
	if (health)
	{
		health->TryTakeDamage(damage);
	}
	if (playerHealth)
	{
		playerHealth->TryTakeDamage(damage);
	}
}

void CollisionSystem::HandleCollision(Entity* first, Entity* second)
{
	auto firstType = first->GetType();
	auto secondType = second->GetType();

	if ((firstType & Player && secondType & Enemy) || (firstType & Enemy && secondType & Player))
	{
		if (firstType & Player)
		{
			auto damage = second->GetComponent<DamageComponent>();
			if (damage)
			{
				ApplyDamage(first, damage->amount);
			}
		}
		else
		{
			auto damage = first->GetComponent<DamageComponent>();
			if (damage)
			{
				ApplyDamage(second, damage->amount);
			}
		}
		HandlePushAway(first, second);
	}
	else if ((firstType & Projectile && secondType & Enemy) || (firstType & Enemy && secondType & Projectile))
	{
		if (firstType & Projectile)
		{
			auto damage = first->GetComponent<DamageComponent>();
			if (damage)
			{
				ApplyDamage(second, damage->amount);
			}
		}
		else
		{
			auto damage = second->GetComponent<DamageComponent>();
			if (damage)
			{
				ApplyDamage(first, damage->amount);
			}
		}
	}
	else if (firstType & Enemy && secondType & Enemy)
	{
		HandlePushAway(first, second);
	}
}

void CollisionSystem::HandlePushAway(Entity* first, Entity* second)
{
	auto firstTransform = first->GetComponent<TransformComponent>();
	auto secondTransform = second->GetComponent<TransformComponent>();

	sf::Vector2f direction = sf::Vector2f(firstTransform->x, firstTransform->y) - sf::Vector2f(secondTransform->x, secondTransform->y);
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 1.0f) return;

	direction /= distance;

	float pushStrength = 1.0f;
	float pushX = direction.x * pushStrength;
	float pushY = direction.y * pushStrength;

	firstTransform->x += pushX;
	firstTransform->y += pushY;
}