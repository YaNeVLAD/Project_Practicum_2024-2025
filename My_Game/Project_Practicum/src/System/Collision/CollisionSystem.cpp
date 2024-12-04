#include "CollisionSystem.h"

#include <iostream>
#include <set>

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto entities = entityManager.GetEntitiesWithComponents<CollisionComponent>();

	if (entities.empty())
	{
		return;
	}

	std::vector<CollisionEvent> events;

	for (auto& entity : entities)
	{
		auto collision = entity->GetComponent<CollisionComponent>();
		sf::FloatRect rect = collision->getRect();

		events.push_back(CollisionEvent(rect.left, CollisionEvent::Type::Open, entity, rect));
		events.push_back(CollisionEvent(rect.left + rect.width, CollisionEvent::Type::Close, entity, rect));
	}

	std::sort(events.begin(), events.end());

	std::set<std::pair<float, Entity*>> active;
	
	for (const auto& event : events)
	{
		if (event.type == CollisionEvent::Type::Open)
		{
			for (const auto& activeRect : active)
			{
				const auto& otherEntity = activeRect.second;

				auto otherCollision = otherEntity->GetComponent<CollisionComponent>();
				sf::FloatRect otherRect = otherCollision->getRect();

				if (!event.rect.intersects(otherRect))
				{
					continue;
				}

				HandleCollision(entityManager, event.entity, otherEntity);
			}
			active.insert(std::pair(event.rect.top, event.entity));
		}
		else
		{
			active.erase(std::pair(event.rect.top, event.entity));
		}
	}
}

void CollisionSystem::ApplyBonus(EntityManager& em, Entity* player, Entity* bonus)
{
	if (player == nullptr || bonus == nullptr)
	{
		return;
	}

	auto bonusComponent = bonus->GetComponent<BonusComponent>();
	auto lifetime = bonus->GetComponent<LifetimeComponent>();

	if (bonusComponent == nullptr || lifetime == nullptr)
	{
		return;
	}

	lifetime->time = -1.f;

	if (bonusComponent->type == BonusComponent::BonusType::Health)
	{
		auto health = player->GetComponent<PlayerHealthComponent>();
		if (health == nullptr)
		{
			return;
		}

		health->AddHealth(50);
	}
	else if (bonusComponent->type == BonusComponent::BonusType::Bomb)
	{
		auto damage = bonus->GetComponent<DamageComponent>();
		auto enemies = em.GetEntitiesWithComponents<HealthComponent>();

		if (enemies.empty() || damage == nullptr)
		{
			return;
		}

 		for (auto& enemy : enemies)
		{
			ApplyDamage(enemy, damage);
		}
	}
}

void CollisionSystem::ApplyDamage(Entity* entity, DamageComponent* damage)
{
	if (entity->GetType() != damage->targetType)
	{
		return;
	}

	auto health = entity->GetComponent<HealthComponent>();
	auto playerHealth = entity->GetComponent<PlayerHealthComponent>();
	auto bossHealth = entity->GetComponent<BossHealthComponent>();

	if (health != nullptr)
	{
		health->TryTakeDamage(damage->amount);
	}
	if (playerHealth != nullptr)
	{
		playerHealth->TryTakeDamage(damage->amount);
	}
	if (bossHealth != nullptr)
	{
		bossHealth->TryTakeDamage(damage->amount);
	}
}

void CollisionSystem::HandleCollision(EntityManager& em, Entity* first, Entity* second)
{
	if (first == nullptr || second == nullptr)
	{
		return;
	}

	auto firstType = first->GetType();
	auto secondType = second->GetType();

	if ((firstType & Player && secondType & Enemy) || (firstType & Enemy && secondType & Player))
	{
		if (firstType & Player)
		{
			auto damage = second->GetComponent<DamageComponent>();
			if (damage != nullptr)
			{
				ApplyDamage(first, damage);
			}
		}
		else
		{
			auto damage = first->GetComponent<DamageComponent>();
			if (damage != nullptr)
			{
				ApplyDamage(second, damage);
			}
		}
	}
	else if (firstType & Projectile || secondType & Projectile)
	{
		if (firstType & Projectile)
		{
			auto damage = first->GetComponent<DamageComponent>();
			if (damage != nullptr)
			{
				ApplyDamage(second, damage);
			}
		}
		else
		{
			auto damage = second->GetComponent<DamageComponent>();
			if (damage != nullptr)
			{
				ApplyDamage(first, damage);
			}
		}
	}
	else if ((firstType & Bonus && secondType & Player) || (firstType & Player && secondType & Bonus))
	{
		if (firstType & Player)
		{
			ApplyBonus(em, first, second);
		}
		else
		{
			ApplyBonus(em, second, first);
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

	float pushStrength = 2.0f;
	float pushX = direction.x * pushStrength;
	float pushY = direction.y * pushStrength;

	firstTransform->x += pushX;
	firstTransform->y += pushY;
}