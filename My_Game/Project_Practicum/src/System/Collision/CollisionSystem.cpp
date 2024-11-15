#include "CollisionSystem.h"
#include <iostream>
#include <set>

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto entities = entityManager.GetEntitiesWithComponents<CollisionComponent>();
	if (entities.empty()) return;

	std::vector<CollisionEvent> events;

	// Создаем события
	for (auto& entity : entities)
	{
		auto collision = entity->GetComponent<CollisionComponent>();
		sf::FloatRect rect = collision->getRect();

		events.push_back(CollisionEvent(rect.left, 1, entity, rect));
		events.push_back(CollisionEvent(rect.left + rect.width, -1, entity, rect));
	}

	// Сортируем события
	std::sort(events.begin(), events.end());

	// Активные прямоугольники по оси Y
	std::set<std::pair<float, Entity*>> active;

	// Обработка событий
	for (const auto& event : events)
	{
		if (event.type == 1) 
		{ // Открытие
			// Проверяем пересечения с активными
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
		{ // Закрытие
			active.erase(std::pair(event.rect.top, event.entity));
		}
	}
}

void CollisionSystem::ApplyDamage(Entity* entity, int damage)
{
	auto health = entity->GetComponent<HealthComponent>();
	if (health)
	{
		health->TryTakeDamage(damage);
	}
}	

void CollisionSystem::HandleCollision(Entity* first, Entity* second)
{
	auto firstType = first->GetType();
	auto secondType = second->GetType();

	if ((firstType & Player && secondType & Enemy) || (firstType & Enemy && secondType & Player)) 
	{
		std::cout << "Player collided with Enemy\n";

		if (firstType & Player)
		{
			ApplyDamage(first, 10);
		}
		else
		{
			ApplyDamage(second, 10);
		}
	}
	else if ((firstType & Projectile && secondType & Enemy) || (firstType & Enemy && secondType & Projectile)) 
	{
		std::cout << "Projectile collided with Enemy\n";

		if (firstType & Projectile)
		{
			ApplyDamage(second, 10);
		}
		else
		{
			ApplyDamage(first, 10);
		}
	}
	else if (firstType & Enemy && secondType & Enemy)
	{
		std::cout << "Enemy collided with Enemy\n";
	}
}