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
	Quaternion q { RotationMatrix({0,1,0}, 60) };
	std::cout << "test: " << q.GetAngle() << "\t" << q.GetAxis() << std::endl;

	Euler e { 30, 10, 15, true };
	std::cout << "test: " << e.ToRotateMatrix() << "\t" << e.ToQuaternion().ToRotateMatrix() << std::endl;
}

GameApp::GameApp() : objs{}, pCamera(std::make_shared<TransitionCamera>()), planets{},
pMouse(Mouse::GetInstance()) {};

void GameApp::Init() {
	Calculate();
	Test();
	pCamera->Get().Move({ 500, 200, 500 });
	pCamera->Get().SetLookAt({ 0, 0, 0 });

	objs.push_back(std::make_shared<Stars>(100));

	planets.push_back(std::make_shared<Sun>(Sun({ 0,0,0 })));
	planets.push_back(std::make_shared<Mercury>(Mercury({ 250,0,0 })));
	planets.push_back(std::make_shared<Venus>(Venus({ 300,0,0 })));
	planets.push_back(std::make_shared<Earth>(Earth({ 350,0,0 })));
	planets.push_back(std::make_shared<Mars>(Mars({ 400,0,0 })));
	planets.push_back(std::make_shared<Jupiter>(Jupiter({ 500,0,0 })));
	planets.push_back(std::make_shared<Saturn>(Saturn({ 700,0,0 })));
	planets.push_back(std::make_shared<Uranus>(Uranus({ 900,0,0 })));
	planets.push_back(std::make_shared<Neptune>(Neptune({ 1000,0,0 })));

	for (auto &p : planets)
	{
		objs.push_back(p);
	}

	pLeftText = std::make_shared<Text>("", Vector2f{ -0.98f, 0.95f });
	objs.push_back(pLeftText);

	pRightText = std::make_shared<Text>("", Vector2f{ 0.75f, 0.95f });
	objs.push_back(pRightText);

	pSpaceship = std::make_shared<Spaceship>(Spaceship({ 800,0,100 }));
	objs.push_back(pSpaceship);

	pSpaceman = std::make_shared<Spaceman>(Vector3f{0, -2, -4}, Vector3f{0, 0, -1}, 2.0f, pSpaceship);
	pSpaceship->BindSpaceman(pSpaceman);
	pSpaceship->BindText(pLeftText);
	pSpaceman->BindText(pRightText);

	pCamera->SetTarget(pSpaceman.get());
}

void GameApp::OnResize() {}

void GameApp::HandleKey()
{
	float speed = 10.0f;
	if (pKeyboard->IsKeyDown('w'))
		pCamera->Get().Move(pCamera->Get().Front() * 0.033 * speed);
	if (pKeyboard->IsKeyDown('s'))
		pCamera->Get().Move(-pCamera->Get().Front() * 0.033 * speed);
	if (pKeyboard->IsKeyDown('a'))
		pCamera->Get().Move(-pCamera->Get().Right() * 0.033 * speed);
	if (pKeyboard->IsKeyDown('d'))
		pCamera->Get().Move(pCamera->Get().Right() * 0.033 * speed);
	if (pKeyboard->IsKeyDown('i'))
		pSpaceship->speed = std::min(pSpaceship->speed * 1.25, 1.0);
	if (pKeyboard->IsKeyDown('k'))
		pSpaceship->speed = std::max(pSpaceship->speed * 0.8, 0.125);
	if (pKeyboard->IsKeyDown('j'))
		pSpaceship->Yaw(0.01);
	if (pKeyboard->IsKeyDown('l'))
		pSpaceship->Yaw(-0.01); 
	if (pKeyboard->IsKeyDown('y'))
		pSpaceship->Pitch(0.01);
	if (pKeyboard->IsKeyDown('h'))
		pSpaceship->Pitch(-0.01);
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
	if (pKeyboard->IsKeyPress(GLUT_KEY_F2))
	{
		if (pCamera->Type() == CameraType::EulerType)
			pCamera->Switch(CameraType::VectorType);
		else
			pCamera->Switch(CameraType::EulerType);
	}
	if (pKeyboard->IsKeyPress(GLUT_KEY_F3))
	{
		pCamera->StartTransition();
	}
	if (pKeyboard->IsKeyPress(GLUT_KEY_F10))
	{
		pCamera->Switch(CameraType::QuaternionType);
	}
}

void GameApp::OnMouseMove(int x, int y)
{
	pMouse->Update(x, y);
	auto yaw = -1.0f * 0.033 * pMouse->deltaX;
	auto pitch = -1.0f * 0.033 * pMouse->deltaY;
	pCamera->Get().Yaw(yaw);
	pCamera->Get().Pitch(pitch);
}

void GameApp::OnMouse(int button, int state, int x, int y)
{
	pMouse->Update(button, state, x, y);
}

void GameApp::OnUpdate(int val)
{
	HandleKey();
	pCamera->Update();

	gluLookAt(pCamera->Get().Position().x(), pCamera->Get().Position().y(), pCamera->Get().Position().z(),
		pCamera->Get().Center().x(), pCamera->Get().Center().y(), pCamera->Get().Center().z(),
		pCamera->Get().Up().x(), pCamera->Get().Up().y(), pCamera->Get().Up().z());
	getError();
	
	Vector4f pos = { 0,0,0,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// Setup light 1
	Vector4f light_ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	Vector4f light_diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4f light_specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.005f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001f);

	Vector4f ship_pos = { pSpaceship->Position().x(), pSpaceship->Position().y(), pSpaceship->Position().z(), 1.0f };
	Vector3f ship_front = pSpaceship->Front();
	Vector4f ship_dir = { ship_front.x(), ship_front.y(), ship_front.z(), 0.0f };
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, ship_dir);
	glLightfv(GL_LIGHT1, GL_POSITION, ship_pos);

	if (pMouse->IsClick())
	{
		Vector3f dir = Unproject({ pMouse->x, pMouse->y });

		float depth = 1024000.0f;
		Planet* selected = nullptr;
		for (auto& p : planets)
		{
			p->isSelected = false;
			float d = p->CheckHit(pCamera->Get().Position(), dir, pCamera->Get().Front());
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