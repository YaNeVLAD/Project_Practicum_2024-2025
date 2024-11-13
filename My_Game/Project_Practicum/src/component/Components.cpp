#include "Components.h"
#include "../entity/weapon/Weapon.h"

void WeaponComponent::AddWeapon(std::unique_ptr<Weapon> weapon)
{
	weapons.push_back(std::move(weapon));
}