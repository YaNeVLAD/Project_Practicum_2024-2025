#include "LifetimeSystem.h"

void LifetimeSystem::Update(EntityManager& entityManager, float deltaTime)
{
    std::vector<Entity*> entitiesToDelete;

    for (auto& entity : entityManager.GetEntitiesWithComponents<LifetimeComponent>())
    {
        auto lifetime = entity->GetComponent<LifetimeComponent>();
        //auto transform = entity->GetComponent<TransformComponent>();

        lifetime->lifetime -= deltaTime;

        if (lifetime->lifetime <= 0.0f) {
            entitiesToDelete.push_back(entity);
            continue;
        }

        //if (IsOutOfBound(transform->x, transform->y))
        //{
        //    entitiesToDelete.push_back(entity);
        //}
    }

    for (auto& entity : entitiesToDelete)
    {
        entityManager.RemoveEntity(entity->GetId());
    }
}

//bool LifetimeSystem::IsOutOfBound(float x, float y)
//{
//    float left = mCamera.getCenter().x - mCamera.getSize().x / 2;
//    float top = mCamera.getCenter().y - mCamera.getSize().y / 2;
//    float right = left + mCamera.getSize().x;
//    float bottom = top + mCamera.getSize().y;
//
//    return x < left || x > right || y < top || y > bottom;
//}
