#pragma once
#include "stdafx.h"
#include "glut.h"
#include "GameApp.h"
#include <iostream>

GameApp::GameApp() : objs{}, pCamera(std::make_shared<Camera>()),
pMouse(Mouse::GetInstance())
{
	pCamera->Move({ 40, 10, 30 });
	pCamera->SetLookAt({ 0, 0, 0 });

	glEnable(GL_DEPTH_TEST);
	objs.push_back(std::make_shared<Line>(Line({ {{0,0,0}, {1,0,0}},{{50, 0, 0}, {1,0,0}} }, LINE_TYPE::Lines, 5)));
	objs.push_back(std::make_shared<Line>(Line({ {{0,0,0}, {0,1,0}},{{0, 50, 0}, {0,1,0}} }, LINE_TYPE::Lines, 5)));
	objs.push_back(std::make_shared<Line>(Line({ {{0,0,0}, {0,0,1}},{{0, 0, 50}, {0,0,1}} }, LINE_TYPE::Lines, 5)));

	objs.push_back(std::make_shared<Stars>(10000));

	pMovingSphere = std::make_shared<MovingSphere>(Vertex3f{ {0, 0, 0}, {1, 0, 0} }, 1.0f, 20, 20);
	objs.push_back(pMovingSphere);
}

void GameApp::OnResize() {}

void GameApp::OnKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		pCamera->Move(pCamera->Front * 0.033 * 5);
		break;
	case 's':
		pCamera->Move(-pCamera->Front * 0.033 * 5);
		break;
	case 'a':
		pCamera->Move(-pCamera->Right * 0.033 * 5);
		break;
	case 'd':
		pCamera->Move(pCamera->Right * 0.033 * 5);
		break;
	case ' ':
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
	auto yaw = -1.0f * 0.033 * pMouse->deltaX;
	auto pitch = -1.0f * 0.033 * pMouse->deltaY;
	pCamera->Yaw(yaw);
	pCamera->Pitch(pitch);
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