#pragma once
#include "stdafx.h"
#include "glut.h"
#include "math.h"
#include "Vector.h"
#include "GameApp.h"

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
private:
	GLApp(int argc, char* argv[]);
	GLApp(const GLApp&) = delete;
	GLApp& operator=(const GLApp&) = delete;
	Vector2f screenSize;
	Vector2f screenPos;
	std::string title;

	GameApp app;

	static void RenderWrapper();
	static void OnResizeWrapper(int w, int h);
	static void OnUpdateWrapper(int val);
};