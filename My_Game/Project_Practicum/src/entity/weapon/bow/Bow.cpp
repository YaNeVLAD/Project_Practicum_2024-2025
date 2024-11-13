#include "Bow.h"
#include "../../../entity/Entity.h"
#include "../../../component/Components.h"

void Bow::Attack(Entity& projectile, const TransformComponent& transform)
{
	projectile.AddComponent<TransformComponent>(
		transform.x, transform.y,
		transform.lastDirection.x * projectileSpeed, 
		transform.lastDirection.y * projectileSpeed
	);
	projectile.AddComponent<ProjectileComponent>(500.0f);
	projectile.AddComponent<DrawableComponent>(10, 10, sf::Color::White);
}

void Bow::LoadTextures()
{

}
