#pragma once
#include "stdafx.h"
#include <memory>

class GameApp
{
public:
	GameApp();
	void OnResize();
	void OnUpdate(int val);
	void OnRender();
};