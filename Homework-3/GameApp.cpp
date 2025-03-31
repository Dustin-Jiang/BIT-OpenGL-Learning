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
    std::cout << "test: KeyCode('w'): " << KeyCode('w') << std::endl;
	std::cout << "test: KeyCode('\\r'): " << KeyCode('\r') << std::endl;
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

	pSpaceship = std::make_shared<Spaceship>(Spaceship({ 40,0,20 }));
    objs.push_back(pSpaceship);
}

void GameApp::OnResize() {}

void GameApp::OnKey(int key, int x, int y)
{
    std::cout << "Key: " << key << std::endl;
	switch (key)
	{
	case KeyCode('w'):
		//pCamera->Move(pCamera->Front * 0.033 * 5);
        pSpaceship->speed = std::min(pSpaceship->speed * 1.25, 1.0);
		break;
	case KeyCode('s'):
		//pCamera->Move(-pCamera->Front * 0.033 * 5);
		pSpaceship->speed = std::max(pSpaceship->speed * 0.8, 0.125);
		break;
	case KeyCode('a'):
		//pCamera->Move(-pCamera->Right * 0.033 * 5);
		pSpaceship->Yaw(0.05);
		break;
	case KeyCode('d'):
		//pCamera->Move(pCamera->Right * 0.033 * 5);
		pSpaceship->Yaw(-0.05);
		break;
	case KeyCode('r'):
		pSpaceship->Pitch(0.05);
		break;
	case KeyCode('f'):
		pSpaceship->Pitch(-0.05);
		break;
	case KeyCode('\r'):
		for (auto& p : planets)
		{
            if (p->isSelected)
            {
                pSpaceship->Follow(p);
				break;
            }
		}
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

void GameApp::OnMouse(int button, int state, int x, int y)
{
    pMouse->Update(button, state, x, y);
}

void GameApp::OnUpdate(int val)
{
	gluLookAt(pCamera->Position.x(), pCamera->Position.y(), pCamera->Position.z(),
		pCamera->Center().x(), pCamera->Center().y(), pCamera->Center().z(),
		pCamera->Up.x(), pCamera->Up.y(), pCamera->Up.z());
	
    if (pMouse->IsClick())
	{
        Vector3f dir = Unproject({ pMouse->x, pMouse->y });

		float depth = 1024000.0f;
        Planet* selected = nullptr;
        for (auto& p : planets)
        {
            p->isSelected = false;
            float d = p->CheckHit(pCamera->Position, dir, pCamera->Front);
			if (d > 0.0 && d < depth)
			{
                depth = d;
                selected = p.get();
			}
        }
        if (selected)
        {
            selected->isSelected = true;
        }
	}

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

Vector3f Unproject(Vector2<int> pos)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	double modelMatrix[16], projMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	pos.y() = viewport[3] - pos.y();

	double x, y, z;

	gluUnProject((double)pos.x(), (double)pos.y(), 0, modelMatrix, projMatrix, viewport, &x, &y, &z);
    Vector3f vFrom = { (float)x, (float)y, (float)z };

    gluUnProject((double)pos.x(), (double)pos.y(), 1, modelMatrix, projMatrix, viewport, &x, &y, &z);
    Vector3f vTo = { (float)x, (float)y, (float)z };

	return (vTo - vFrom).Normalized();
}