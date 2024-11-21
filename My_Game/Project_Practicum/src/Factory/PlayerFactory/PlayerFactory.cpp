#include "PlayerFactory.h"

#include "../../Entity/Weapon/MagicCharge/MagicCharge.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../Manager/Entity/EntityManager.h"
#include "../../Entity/Weapon/Book/Book.h"

void PlayerFactory::Create(EntityManager& entityManager)
{
	auto& player = entityManager.CreateEntity(EntityType::Player);
	player.AddComponent<TransformComponent>(0.0f, 0.0f, 0.0f, 0.0f);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();

	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<MagicCharge>());

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20, 20); 
	player.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 35));

	std::vector<sf::Texture> frames = TextureManager::GetTextures("assets/character/Walk.png", 128, 128);
	player.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, false);
	player.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(1.1f, 1.1f));

	player.AddComponent<PlayerHealthComponent>(100, 1.0f);

	player.AddComponent<ExperienceComponent>(100);
}