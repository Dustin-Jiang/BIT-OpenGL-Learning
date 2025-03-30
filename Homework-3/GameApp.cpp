#pragma once
#include "stdafx.h"
#include "glut.h"
#include "GameApp.h"
#include <iostream>

#include "Keyboard.h"

std::string GBKToUTF8(const std::string& gbkStr) {
	int len = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
	std::vector<wchar_t> wstr(len);
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, wstr.data(), len);

	len = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, NULL, 0, NULL, NULL);
	std::vector<char> utf8Str(len);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, utf8Str.data(), len, NULL, NULL);

	return std::string(utf8Str.data());
}

void Test()
{
	std::cout << "Test: RotationMatrix " << RotationMatrix({ 0, 1, 0 }, 90) << std::endl;
	std::cout << "Test: RotatedPosition " << RotatedPosition(Vector3f {1, 0, 0}, RotationMatrix({0, 1, 0}, 90)) << std::endl;
}

GameApp::GameApp() : objs{}, pCamera(std::make_shared<Camera>()), planets{},
pMouse(Mouse::GetInstance())
{
	CalculateMatrix();
	Test();
	pCamera->Move({ 100, 40, 100 });
	pCamera->SetLookAt({ 0, 0, 0 });

	glEnable(GL_DEPTH_TEST);
	/*objs.push_back(std::make_shared<Arrow>(Arrow({ {{0,0,0}, {1,0,0}},{{50, 0, 0}, {1,0,0}} })));
	objs.push_back(std::make_shared<Arrow>(Arrow({ {{0,0,0}, {0,1,0}},{{0, 50, 0}, {0,1,0}} })));
	objs.push_back(std::make_shared<Arrow>(Arrow({ {{0,0,0}, {0,0,1}},{{0, 0, 50}, {0,0,1}} })));*/

	objs.push_back(std::make_shared<Stars>(100));

    pMovingSphere = std::make_shared<MovingSphere>(Vertex3f{ {0, 0, 0}, {1, 0, 0} }, 1.0f, 20, 20);
    objs.push_back(pMovingSphere);

	planets.push_back(std::make_shared<Sun>(Sun({ 0,0,0 })));
    planets.push_back(std::make_shared<Mercury>(Mercury({ 25,0,0 })));
    planets.push_back(std::make_shared<Venus>(Venus({ 30,0,0 })));
    planets.push_back(std::make_shared<Earth>(Earth({ 35,0,0 })));
    planets.push_back(std::make_shared<Mars>(Mars({ 40,0,0 })));
    planets.push_back(std::make_shared<Jupiter>(Jupiter({ 50,0,0 })));
    planets.push_back(std::make_shared<Saturn>(Saturn({ 65,0,0 })));
    planets.push_back(std::make_shared<Uranus>(Uranus({ 80,0,0 })));
    planets.push_back(std::make_shared<Neptune>(Neptune({ 90,0,0 })));

    for (auto &p : planets)
    {
      objs.push_back(p);
    }
}

void GameApp::OnResize() {}

void GameApp::OnKey(int key, int x, int y)
{
	switch (key)
	{
	case KeyCode('w'):
		pCamera->Move(pCamera->Front * 0.033 * 5);
		break;
	case KeyCode('s'):
		pCamera->Move(-pCamera->Front * 0.033 * 5);
		break;
	case KeyCode('a'):
		pCamera->Move(-pCamera->Right * 0.033 * 5);
		break;
	case KeyCode('d'):
		pCamera->Move(pCamera->Right * 0.033 * 5);
		break;
    case KeyCode(GLUT_KEY_F1):
        for (auto &p : planets)
        {
            p->isWire = !p->isWire;
        }
        break;
	case KeyCode('i'):
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{0, 0.05, 0}; });
		break;
	case KeyCode('k'):
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0, -0.05, 0 }; });
		break;
	case KeyCode('j'):
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ -0.05, 0, 0 }; });
		break;
    case KeyCode('l'):
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0.05, 0, 0 }; });
		break;
    case KeyCode('u'):
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0, 0, 0.05 }; });
		break;
    case KeyCode('o'):
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0, 0, -0.05 }; });
		break;
    case KeyCode('y'):
		pMovingSphere->CalcVelocity([](Vector3f v) { return v * 1.2; });
		break;
    case KeyCode('h'):
		pMovingSphere->CalcVelocity([](Vector3f v) { return v * 0.8; });
		break;
    case KeyCode(' '):
		pMovingSphere->ResetVelocity();
		pMovingSphere->ResetPosition();
		break;
	default:
		break;
	}
}

void GameApp::OnMouseMove(int x, int y)
{
	pMouse->Update(x, y);
}

void GameApp::OnUpdate(int val)
{
	gluLookAt(pCamera->Position.x(), pCamera->Position.y(), pCamera->Position.z(),
		pCamera->Center().x(), pCamera->Center().y(), pCamera->Center().z(),
		pCamera->Up.x(), pCamera->Up.y(), pCamera->Up.z());

	for (auto o : objs)
	{
		o->Update(val);
	}
}

void GameApp::OnRender()
{
	for (auto o : objs)
	{
		o->Draw();
	}
}