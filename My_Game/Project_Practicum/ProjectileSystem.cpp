#include "ProjectileSystem.h"

void ProjectileSystem::Update(EntityManager& entityManager, float deltaTime)
{
    std::vector<Entity*> projectilesToDelete;

    for (auto& entity : entityManager.GetEntities()) 
    {
        auto position = entity.GetComponent<TransformComponent>();
        auto projectile = entity.GetComponent<ProjectileComponent>();

        if (position && projectile)
        {
            if (IsOutOfBound(position->x, position->y)) 
            {
                projectilesToDelete.push_back(&entity);
            }
        }
    }

    for (auto& projectile : projectilesToDelete)
    {
        entityManager.RemoveEntity(projectile->GetId());
    }
}

bool ProjectileSystem::IsOutOfBound(float x, float y)
{
    return x < 0 || x > 800 || y < 0 || y > 600;
}
