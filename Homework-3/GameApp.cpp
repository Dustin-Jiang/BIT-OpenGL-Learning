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

void CalculateMatrix() {
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
    if (lines[i] == L"矩阵相乘\r") {
      out << "矩阵相乘" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
      float n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15;
      if (swscanf_s(dataLine.c_str(),
        L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f %f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
        &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15,
        &n0, &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9, &n10, &n11, &n12, &n13, &n14, &n15) != 32) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Matrix4f mtx1{ m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 },
          mtx2{ n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15 };
        out << mtx1 << "\t" << mtx2 << "\t" << mtx2 * mtx1 << std::endl;
      }
      processed = true;
    }
    // 向量点乘
    else if (lines[i] == L"矩阵乘向量\r") {
      out << "矩阵乘向量" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
      float n0, n1, n2;
      if (swscanf_s(dataLine.c_str(),
        L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f %f,%f,%f",
        &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15,
        &n0, &n1, &n2) != 19) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Matrix4f mtx1{ m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 };
        HVector3f mtx2 { n0, n1, n2 };
        Matrix3f mtx3 { m0, m1, m2, m4, m5, m6, m8, m9, m10 };
        out << mtx1 << "\t" << mtx2 << "\t" << mtx2 * mtx3 << std::endl;
      }
      processed = true;
    }
    // 向量叉乘
    else if (lines[i] == L"矩阵乘位置\r") {
      out << "矩阵乘位置" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
      float n0, n1, n2;
      if (swscanf_s(dataLine.c_str(),
        L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f %f,%f,%f",
        &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15,
        &n0, &n1, &n2) != 19) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Matrix4f mtx1{ m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 };
        HVector3f mtx2{ n0, n1, n2 };
        HVector4f mtx3{ n0, n1, n2, 1 };
        auto result4f = mtx3 * mtx1;
        VVector3f result3f = { result4f(0, 0), result4f(0, 1), result4f(0, 2) };
        out << mtx1 << "\t" << mtx2 << "\t" << result3f << std::endl;
      }
      processed = true;
    }
    else if (lines[i] == L"矩阵设置旋转\r") {
      out << "矩阵设置旋转" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float x1, y1, z1;
      float deg;
      if (swscanf_s(dataLine.c_str(), L"%f %f,%f,%f",
        &deg, &x1, &y1, &z1) != 4) {
        out << "数据格式错误" << std::endl;
      }
      else {
        float rad = deg * PI / 180;
        Vector3f v1{ x1, y1, z1 }, v = v1.Normalized();
        VVector3f u { v.x(), v.y(), v.z() };
        Matrix3f ux{ 0 , -v.z(), v.y(), v.z(), 0, -v.x(), -v.y(), -v.x(), 0};
        Matrix3f i{ 1,0,0,0,1,0,0,0,1 };
        auto result3f = i * cos(rad) + ux * sin(rad) + (u * u.Transpose()) * (1 - cos(rad));
        Matrix4f result{
          result3f(0,0), result3f(1,0), result3f(2,0), 0,
          result3f(0,1), result3f(1,1), result3f(2,1), 0,
          result3f(0,2), result3f(1,2), result3f(2,2), 0,
          0, 0, 0, 1 };
        out << deg << "\t" << v1 << "\t" << result << std::endl;
      }
      processed = true;
    }
    else if (lines[i] == L"矩阵设置平移\r") {
      out << "矩阵设置平移" << std::endl;
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
        VVector3f v1{ x1, y1, z1 };
        Matrix4f m1{ 1,0,0,0,0,1,0,0,0,0,1,0,x1, y1, z1,1 };
        out << v1 << "\t" << m1 << std::endl;
      }
      processed = true;
    }
    else if (lines[i] == L"矩阵设置缩放\r") {
      out << "矩阵设置缩放" << std::endl;
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
        VVector3f v1{ x1, y1, z1 };
        Matrix4f m1{ x1,0,0,0,0, y1,0,0,0,0,z1,0,0,0,0,1 };
        out << v1 << "\t" << m1 << std::endl;
      }
      processed = true;
    }
    // 向量投影
    else if (lines[i] == L"矩阵求逆\r") {
      out << "矩阵求逆" << std::endl;
      if (++i >= lines.size()) {
        out << "数据行缺失" << std::endl;
        continue;
      }
      std::wstring dataLine = lines[i];
      float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
      if (swscanf_s(dataLine.c_str(),
        L"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
        &m0, &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8, &m9, &m10, &m11, &m12, &m13, &m14, &m15) != 16) {
        out << "数据格式错误" << std::endl;
      }
      else {
        Matrix4f mtx1{ m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 };
        out << mtx1 << "\t" << mtx1.Inverse() << std::endl;
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
	CalculateMatrix();
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