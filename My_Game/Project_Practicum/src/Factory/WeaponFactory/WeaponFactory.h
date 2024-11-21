#pragma once

#include "../../Entity/Weapon/Weapon.h"
#include <memory>

class WeaponFactory
{
public:
	static std::unique_ptr<Weapon> Create(const std::string& weaponName);
};