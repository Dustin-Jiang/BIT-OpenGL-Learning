#pragma once
#include "stdafx.h"
#include "glut.h"
#include "GameApp.h"

#define A2R(x) (x/180.0*3.14159)
void DrawBall(float R)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = -90; i < 90; i += 3)
	{
		for (int j = 0; j < 360; j += 3)
		{
			float r = R * cos(A2R(i));
			float x = r * sin(A2R(j));
			float z = r * cos(A2R(j));
			float y = R * sin(A2R(i));
			glVertex3f(x, y, z);
		}
	}
	glEnd();
}

GameApp::GameApp()
{
	objs.push_back(std::make_shared<Line>(Line({ {{0,0,0}, {1,1,0}},{{50, 0, 0}, {1,1,0}} }, LINE_TYPE::Lines, 5)));
	objs.push_back(std::make_shared<Line>(Line({ {{0,0,0}, {1,1,0}},{{0, 50, 0}, {1,1,0}} }, LINE_TYPE::Lines, 5)));
	objs.push_back(std::make_shared<Line>(Line({ {{0,0,0}, {1,1,0}},{{0, 0, 50}, {1,1,0}} }, LINE_TYPE::Lines, 5)));
}

void GameApp::OnResize() {}

void GameApp::OnUpdate(int val) {}

void GameApp::OnRender()
{
	glPushMatrix();
	glTranslatef(0, 0, -20);
	DrawBall(10);
	for (auto o : objs)
	{
		o->Draw();
	}
	glPopMatrix();
}