#include "ProjectileSystem.h"

void ProjectileSystem::Update(EntityManager& entityManager, float deltaTime)
{
    std::vector<Entity*> projectilesToDelete;

    for (auto& entity : entityManager.GetEntitiesWithComponents<TransformComponent, ProjectileComponent>())
    {
        auto projectile = entity->GetComponent<ProjectileComponent>();
        auto transform = entity->GetComponent<TransformComponent>();

        projectile->lifetime -= deltaTime;

        if (projectile->lifetime <= 0.0f) {
            projectilesToDelete.push_back(entity);
            continue;
        }

        if (IsOutOfBound(transform->x, transform->y))
        {
            projectilesToDelete.push_back(entity);
        }
    }

    for (auto& projectile : projectilesToDelete)
    {
        entityManager.RemoveEntity(projectile->GetId());
    }
}

bool ProjectileSystem::IsOutOfBound(float x, float y)
{
    float left = mCamera.getCenter().x - mCamera.getSize().x / 2;
    float top = mCamera.getCenter().y - mCamera.getSize().y / 2;
    float right = left + mCamera.getSize().x;
    float bottom = top + mCamera.getSize().y;

    return x < left || x > right || y < top || y > bottom;
}
