#include "SystemManager.h"

std::vector<std::unique_ptr<System>>& SystemManager::GetSystems()
{
	return mSystems;
}