#pragma once
#include "stdafx.h"
#include "Geometry.h"
#include "Camera.h"
#include "Mouse.h"

#include "Stars.h"
#include "MovingSphere.h"

#include <memory>
#include <vector>

class GameApp
{
public:
	GameApp();
	void OnResize();
	void OnUpdate(int val);
	void OnRender();
	void OnKey(unsigned char key, int x, int y);
	void OnMouseMove(int x, int y);

private:
	std::vector<pDrawable> objs;
	std::shared_ptr<Camera> pCamera;
	std::shared_ptr<Mouse> pMouse;
};