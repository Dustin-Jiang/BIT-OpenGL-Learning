#pragma once
#include "stdafx.h"
#include "glut.h"
#include "GameApp.h"

GameApp::GameApp() {}

void GameApp::OnResize() {}

void GameApp::OnUpdate(int val) {}

void GameApp::OnRender()
{
	glPushMatrix();
	glTranslatef(0, 0, -20);
	glPopMatrix();
}