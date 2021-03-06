#pragma once
#include "IComponentManager.h"
#include "../Component/LightComponent.h"

class ILightComponentManager : public IComponentManager
{
public:
	INNO_CLASS_INTERFACE_NON_COPYABLE(ILightComponentManager);
	virtual const std::vector<LightComponent*>& GetAllComponents() = 0;
	virtual const LightComponent* GetSun() = 0;
	virtual const std::vector<AABB>& GetSunSplitAABB() = 0;
	virtual const std::vector<Mat4>& GetSunViewMatrices() = 0;
	virtual const std::vector<Mat4>& GetSunProjectionMatrices() = 0;
};