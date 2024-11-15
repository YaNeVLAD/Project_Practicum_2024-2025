#include "SystemManager.h"

std::vector<std::unique_ptr<System>>& SystemManager::GetAllSystems()
{
	return mSystems;
}

std::vector<std::unique_ptr<IUpdateSystem>>& SystemManager::GetUpdateSystems()
{
	return mUpdateSystems;
}

std::vector<std::unique_ptr<IRenderSystem>>& SystemManager::GetRenderSystems()
{
	return mRenderSystems;
}