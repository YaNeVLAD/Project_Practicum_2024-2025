#include "CollisionSystem.h"
#include <set>

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto entities = entityManager.GetEntitiesWithComponents<CollisionComponent>();
	if (entities.empty()) return;

	std::vector<CollisionEvent> events;

	// ������� �������
	for (auto& entity : entities)
	{
		auto collision = entity->GetComponent<CollisionComponent>();
		sf::FloatRect rect = collision->getRect();

		events.push_back(CollisionEvent(rect.left, 1, entity, rect));
		events.push_back(CollisionEvent(rect.left + rect.width, -1, entity, rect));
	}

	// ��������� �������
	std::sort(events.begin(), events.end());

	// �������� �������������� �� ��� Y
	std::set<std::pair<float, Entity*>> active;

	// ��������� �������
	for (const auto& event : events)
	{
		if (event.type == 1) { // ��������
			// ��������� ����������� � ���������
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
			active.insert({ event.rect.top, event.entity });
		}
		else { // ��������
			active.erase({ event.rect.top, event.entity });
		}
	}
}

void CollisionSystem::HandleCollision(Entity* first, Entity* second)
{
	std::cout << "Collision detected between Entity " << first->GetId() << " and Entity " << second->GetId() << std::endl;
}