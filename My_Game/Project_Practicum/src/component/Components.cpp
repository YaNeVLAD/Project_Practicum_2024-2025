#include "Components.h"

#include "../entity/weapon/Weapon.h"
#include "../Entity/Entity.h"

void WeaponComponent::AddWeapon(std::shared_ptr<Weapon> weapon)
{
	weapon->Upgrade(1);
	weapons.push_back(std::move(weapon));
}