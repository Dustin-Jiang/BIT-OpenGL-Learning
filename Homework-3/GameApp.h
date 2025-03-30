#pragma once
#include "stdafx.h"
#include "Geometry.h"
#include "Camera.h"
#include "Mouse.h"

#include "Stars.h"
#include "Planet.h"
#include "Spaceship.h"
#include "MovingSphere.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include <windows.h>

#include "Matrix.h"

#include <memory>
#include <vector>

class GameApp
{
public:
	GameApp();
	void OnResize();
	void OnUpdate(int val);
	void OnRender();
	void OnKey(int key, int x, int y);
	void OnMouseMove(int x, int y);
    void OnMouse(int button, int state, int x, int y);

private:
	std::vector<pDrawable> objs;
	std::shared_ptr<Camera> pCamera;
	std::shared_ptr<Mouse> pMouse;
	std::vector<std::shared_ptr<Planet>> planets;

	std::shared_ptr<MovingSphere> pMovingSphere;
    std::shared_ptr<Spaceship> pSpaceship;
};

void CalculateMatrix(void);
Vector3f Unproject(Vector2<int> pos);