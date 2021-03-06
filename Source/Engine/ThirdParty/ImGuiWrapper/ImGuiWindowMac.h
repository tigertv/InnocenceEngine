#pragma once
#include "IImGuiWindow.h"

class ImGuiWindowMac : public IImGuiWindow
{
public:
	INNO_CLASS_CONCRETE_NON_COPYABLE(ImGuiWindowMac);

	bool setup() override;
	bool initialize() override;
	bool newFrame() override;
	bool terminate() override;
};
