#pragma once

#include "../Manager/Entity/EntityManager.h"
#include "SFML/Graphics.hpp"

class System
{
public:
	~System() = default;
};

class IRenderSystem : public System
{
public:
	virtual void Render(EntityManager& entityManager, float deltaTime) = 0;
};

class IUpdateSystem : public System
{
public:
	virtual void Update(EntityManager& entityManager, float deltaTime) = 0;
};