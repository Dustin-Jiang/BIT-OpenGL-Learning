#pragma once
#include "stdafx.h"
#include "Geometry.h"
#include "Camera.h"
#include "Mouse.h"

#include "Stars.h"
#include "Planet.h"
#include "Spaceship.h"
#include "Spaceman.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include <windows.h>

#include "Matrix.h"
#include "Euler.h"
#include "Quaternion.h"
#include "Keyboard.h"

#include <memory>
#include <vector>

class GameApp
{
public:
	GameApp();
	void OnResize();
	void OnUpdate(int val);
	void OnRender();
	void OnMouseMove(int x, int y);
    void OnMouse(int button, int state, int x, int y);
	void HandleKey();

private:
	std::vector<pDrawable> objs;
	std::shared_ptr<TransitionCamera> pCamera;
	Keyboard* pKeyboard = Keyboard::GetInstance();
	std::shared_ptr<Mouse> pMouse;
	std::vector<std::shared_ptr<Planet>> planets;

	std::shared_ptr<Spaceship> pSpaceship;
	std::shared_ptr<Spaceman> pSpaceman;
};

void Calculate(void);
Vector3f Unproject(Vector2<int> pos);