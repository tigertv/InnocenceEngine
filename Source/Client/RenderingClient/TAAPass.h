#pragma once
#include "../../Engine/RenderingServer/IRenderingServer.h"

namespace TAAPass
{
	bool Setup();
	bool Initialize();
	bool PrepareCommandList(IResourceBinder* input);
	bool ExecuteCommandList();
	bool Terminate();

	RenderPassDataComponent* GetRPDC();
	ShaderProgramComponent* GetSPC();
};
