#pragma once
#include "stdafx.h"
#include "glut.h"
#include "GameApp.h"
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include <windows.h>

std::string GBKToUTF8(const std::string& gbkStr) {
	int len = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
	std::vector<wchar_t> wstr(len);
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, wstr.data(), len);

	len = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, NULL, 0, NULL, NULL);
	std::vector<char> utf8Str(len);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, utf8Str.data(), len, NULL, NULL);

	return std::string(utf8Str.data());
}

void CalculateVector()
{
	std::string filename = "test.txt";
	std::ifstream fd(filename, std::ios::binary);

	if (!fd.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::string gbkContent((std::istreambuf_iterator<char>(fd)), std::istreambuf_iterator<char>());
	std::string result = GBKToUTF8(gbkContent);

	std::vector<std::string> lines {};
	std::istringstream iss(result);

	std::string line;
	while (std::getline(iss, line)) {
		lines.push_back(line);
	}

	std::string outfile = "out.txt";
	std::ofstream out(outfile);

	int i = 0;
	while (i < line.length())
	{
		if (lines[i] == "向量相加\r")
		{
			i++;
			out << "向量相加" << std::endl;
			float x1, y1, z1, x2, y2, z2;
			sscanf_s(lines[i].c_str(), "%f,%f,%f %f,%f,%f", &x1, &y1, &z1, &x2, &y2, &z2);
			Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
			out << v1 << "\t" << v2 << "\t" << v1 + v2 << std::endl;
		}
		else if (lines[i] == "向量点乘\r")
		{
			i++;
			out << "向量点乘" << std::endl;
			float x1, y1, z1, x2, y2, z2;
			sscanf_s(lines[i].c_str(), "%f,%f,%f %f,%f,%f", &x1, &y1, &z1, &x2, &y2, &z2);
			Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
			out << v1 << "\t" << v2 << "\t" << v1.Dot(v2) << std::endl;
		}
		else if (lines[i] == "向量叉乘\r")
		{
			i++;
			out << "向量叉乘" << std::endl;
			float x1, y1, z1, x2, y2, z2;
			sscanf_s(lines[i].c_str(), "%f,%f,%f %f,%f,%f", &x1, &y1, &z1, &x2, &y2, &z2);
			Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
			out << v1 << "\t" << v2 << "\t" << v1.Cross(v2) << std::endl;
		}
		else if (lines[i] == "向量标准化\r")
		{
			i++;
			out << "向量标准化" << std::endl;
			float x1, y1, z1;
			sscanf_s(lines[i].c_str(), "%f,%f,%f", &x1, &y1, &z1);
			Vector3f v1{ x1, y1, z1 };
			out << v1 << "\t" << v1.Normalized() << std::endl;
		}
		else if (lines[i] == "向量求模\r")
		{
			i++;
			out << "向量求模" << std::endl;
			float x1, y1, z1;
			sscanf_s(lines[i].c_str(), "%f,%f,%f", &x1, &y1, &z1);
			Vector3f v1{ x1, y1, z1 };
			out << v1 << "\t" << v1.Length() << std::endl;
		}
		else if (lines[i] == "向量投影\r")
		{
			i++;
			out << "向量投影" << std::endl;
			float x1, y1, z1, x2, y2, z2;
			sscanf_s(lines[i].c_str(), "%f,%f,%f %f,%f,%f", &x1, &y1, &z1, &x2, &y2, &z2);
			Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
			out << v1 << "\t" << v2 << "\t" << v1.ProjectTo(v2) << std::endl;
		}
		i++;
	}
}

GameApp::GameApp() : objs{}, pCamera(std::make_shared<Camera>()),
pMouse(Mouse::GetInstance())
{
	CalculateVector();
	pCamera->Move({ 40, 10, 30 });
	pCamera->SetLookAt({ 0, 0, 0 });

	glEnable(GL_DEPTH_TEST);
	objs.push_back(std::make_shared<Arrow>(Arrow({ {{0,0,0}, {1,0,0}},{{50, 0, 0}, {1,0,0}} })));
	objs.push_back(std::make_shared<Arrow>(Arrow({ {{0,0,0}, {0,1,0}},{{0, 50, 0}, {0,1,0}} })));
	objs.push_back(std::make_shared<Arrow>(Arrow({ {{0,0,0}, {0,0,1}},{{0, 0, 50}, {0,0,1}} })));

	objs.push_back(std::make_shared<Stars>(100));

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
	case 'i':
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{0, 0.05, 0}; });
		break;
	case 'k':
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0, -0.05, 0 }; });
		break;
	case 'j':
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ -0.05, 0, 0 }; });
		break;
	case 'l':
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0.05, 0, 0 }; });
		break;
	case 'u':
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0, 0, 0.05 }; });
		break;
	case 'o':
		pMovingSphere->CalcVelocity([](Vector3f v){ return v + Vector3f{ 0, 0, -0.05 }; });
		break;
	case 'y':
		pMovingSphere->CalcVelocity([](Vector3f v) { return v * 1.2; });
		break;
	case 'h':
		pMovingSphere->CalcVelocity([](Vector3f v) { return v * 0.8; });
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