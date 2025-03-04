#pragma once
#include "stdafx.h"
#include "Geometry.h"

#include <memory>
#include <vector>

class GameApp
{
public:
	GameApp();
	void OnResize();
	void OnUpdate(int val);
	void OnRender();

private:
	std::vector<pDrawable> objs;
};