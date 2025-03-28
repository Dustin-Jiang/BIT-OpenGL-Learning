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

void CalculateVector() {
  std::string filename = "test.txt";
  std::ifstream file(filename, std::ios::binary);
  if (!file)
    throw std::runtime_error("无法打开文件");

  // 读取文件内容到缓冲区
  std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());

  // 使用MultiByteToWideChar进行编码转换（包含两次调用）
  int requiredSize = MultiByteToWideChar(54936, 0, buffer.data(), static_cast<int>(buffer.size()), NULL, 0);
  if (requiredSize == 0) throw std::runtime_error("转换失败: 需要的空间计算错误");

  std::wstring result(requiredSize, L'\0');
  int convertedSize = MultiByteToWideChar(54936, 0, buffer.data(), static_cast<int>(buffer.size()), &result[0], requiredSize);
  if (convertedSize == 0) throw std::runtime_error("转换失败");

  // 将结果拆分为行
  std::vector<std::wstring> lines;
  std::wistringstream iss(result);
  std::wstring line;
  while (std::getline(iss, line)) {
    lines.push_back(line);
  }

  std::string outfile = "out.txt";
  std::ofstream out(outfile);
  if (!out.is_open()) throw std::runtime_error("无法打开输出文件");

  size_t i = 0; // 使用size_t作为索引避免符号问题

  while (i < lines.size()) {
    bool processed = false;

    // 向量相加
    if (lines[i] == L"向量相加\r") {
      out << "向量相加" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1, x2, y2, z2;
      if (swscanf_s(dataLine.c_str(), L"%f,%f,%f %f,%f,%f",
        &x1, &y1, &z1, &x2, &y2, &z2) != 6) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
        out << v1 << "\t" << v2 << "\t" << v1 + v2 << std::endl;
      }
      processed = true;
    }
    // 向量点乘
    else if (lines[i] == L"向量点乘\r") {
      out << "向量点乘" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1, x2, y2, z2;
      if (swscanf_s(dataLine.c_str(), L"%f,%f,%f %f,%f,%f",
        &x1, &y1, &z1, &x2, &y2, &z2) != 6) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
        out << v1 << "\t" << v2 << "\t" << v1.Dot(v2) << std::endl;
      }
      processed = true;
    }
    // 向量叉乘
    else if (lines[i] == L"向量叉乘\r") {
      out << "向量叉乘" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1, x2, y2, z2;
      if (swscanf_s(dataLine.c_str(), L"%f,%f,%f %f,%f,%f",
        &x1, &y1, &z1, &x2, &y2, &z2) != 6) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
        out << v1 << "\t" << v2 << "\t" << v1.Cross(v2) << std::endl;
      }
      processed = true;
    }
    // 向量标准化
    else if (lines[i] == L"向量标准化\r") {
      out << "向量标准化" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1;
      if (swscanf_s(dataLine.c_str(), L"%f,%f,%f",
        &x1, &y1, &z1) != 3) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Vector3f v1{ x1, y1, z1 };
        out << v1 << "\t" << v1.Normalized() << std::endl;
      }
      processed = true;
    }
    // 向量求模
    else if (lines[i] == L"向量求模\r") {
      out << "向量求模" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1;
      if (swscanf_s(dataLine.c_str(), L"%f,%f,%f",
        &x1, &y1, &z1) != 3) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Vector3f v1{ x1, y1, z1 };
        out << v1 << "\t" << v1.Length() << std::endl;
      }
      processed = true;
    }
    // 向量投影
    else if (lines[i] == L"向量投影\r") {
      out << "向量投影" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1, x2, y2, z2;
      if (swscanf_s(dataLine.c_str(), L"%f,%f,%f %f,%f,%f",
        &x1, &y1, &z1, &x2, &y2, &z2) != 6) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Vector3f v1{ x1, y1, z1 }, v2{ x2, y2, z2 };
        out << v1 << "\t" << v2 << "\t" << v1.ProjectTo(v2) << std::endl;
      }
      processed = true;
    }

    if (processed) i += 1;
    else ++i;   // 未处理则跳转一行
  }

  out.close();
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