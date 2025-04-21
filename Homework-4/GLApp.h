#pragma once
#include "stdafx.h"
#include "glut.h"
#include "math.h"
#include "Vector.h"
#include "GameApp.h"
#include "Keyboard.h"

#include <string>

class GLApp
{
public:
	static GLApp* GetInstance(int argc = 0, char* argv[] = nullptr) {
		static GLApp instance(argc, argv);
		return &instance;
	}
	void Init(const std::string title, Vector2f screenSize, Vector2f screenPos);
	void Run();
	void OnUpdate(int val);
	void OnRender();
	void OnResize(Vector2f screenSize);
	void OnResize(int w, int h);
	void OnKeyDown(Key key, int x, int y);
	void OnKeyUp(Key key, int x, int y);  // 添加按键抬起处理函数
	void OnMouseMove(int x, int y);
    void OnMouse(int button, int state, int x, int y);
private:
	GLApp(int argc, char* argv[]);
	GLApp(const GLApp&) = delete;
	GLApp& operator=(const GLApp&) = delete;
	Vector2f screenSize;
	Vector2f screenPos;
	std::string title;

	GameApp app;
	Keyboard* pKeyboard = Keyboard::GetInstance();
	const int interval = 1;

	static void RenderWrapper();
	static void OnResizeWrapper(int w, int h);
	static void OnUpdateWrapper(int val);
	static void OnKeyDownWrapper(unsigned char key, int x, int y);
	static void OnKeyUpWrapper(unsigned char key, int x, int y);  // 添加按键抬起的回调包装函数
	static void OnSpecialDownWrapper(int key, int x, int y);
	static void OnSpecialUpWrapper(int key, int x, int y);  // 添加特殊键抬起的回调包装函数
	static void OnMouseMoveWrapper(int x, int y);
	static void OnMouseWrapper(int button, int state, int x, int y);
};