#pragma once
#include "stdafx.h"
#include "glut.h"
#include "GameApp.h"
#include <iostream>

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

GameApp::GameApp() : objs{}, pCamera(std::make_shared<VectorCamera>()), planets{},
pMouse(Mouse::GetInstance())
{
	Calculate();
	Test();
	pCamera->Move({ 100, 40, 100 });
	pCamera->SetLookAt({ 0, 0, 0 });

	glEnable(GL_DEPTH_TEST);

	objs.push_back(std::make_shared<Stars>(100));

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

void GameApp::HandleKey()
{
	if (pKeyboard->IsKeyDown('w'))
		pCamera->Move(pCamera->Front * 0.033 * 5);
	if (pKeyboard->IsKeyDown('s'))
		pCamera->Move(-pCamera->Front * 0.033 * 5);
	if (pKeyboard->IsKeyDown('a'))
		pCamera->Move(-pCamera->Right * 0.033 * 5);
	if (pKeyboard->IsKeyDown('d'))
		pCamera->Move(pCamera->Right * 0.033 * 5);
	if (pKeyboard->IsKeyDown('i'))
		pSpaceship->speed = std::min(pSpaceship->speed * 1.25, 1.0);
	if (pKeyboard->IsKeyDown('k'))
		pSpaceship->speed = std::max(pSpaceship->speed * 0.8, 0.125);
	if (pKeyboard->IsKeyDown('j'))
		pSpaceship->Yaw(0.05);
	if (pKeyboard->IsKeyDown('l'))
		pSpaceship->Yaw(-0.05); 
	if (pKeyboard->IsKeyDown('y'))
		pSpaceship->Pitch(0.05);
	if (pKeyboard->IsKeyDown('h'))
		pSpaceship->Pitch(-0.05);
	if (pKeyboard->IsKeyDown('\r'))
	{
		for (auto& p : planets)
		{
			if (p->isSelected)
			{
				pSpaceship->Follow(p);
				break;
			}
		}
	}
	if (pKeyboard->IsKeyPress(GLUT_KEY_F1))
	{
		for (auto& p : planets)
		{
			p->isWire = !p->isWire;
		}
	}
}

void GameApp::OnMouseMove(int x, int y)
{
	pMouse->Update(x, y);
	auto yaw = -1.0f * 0.033 * pMouse->deltaX;
	auto pitch = -1.0f * 0.033 * pMouse->deltaY;
	pCamera->Yaw(yaw);
	pCamera->Pitch(pitch);
}

void GameApp::OnMouse(int button, int state, int x, int y)
{
    pMouse->Update(button, state, x, y);
}

void GameApp::OnUpdate(int val)
{
	HandleKey();
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