#pragma execution_character_set("uft-8")
#pragma once
#include "stdafx.h"
#include "GLApp.h"
#include "Keyboard.h"

GLApp::GLApp(int argc, char* argv[]): app()
{
	glutInit(&argc, argv);
}

void GLApp::Init(
	const std::string title = "图形作业模板",
	Vector2f screenSize = {1024, 768},
	Vector2f screenPos = {100, 100})
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(screenPos.x(), screenPos.y());
	glutInitWindowSize(screenSize.x(), screenSize.y());

	this->screenSize = screenSize;
	this->screenPos = screenPos;
	this->title = title;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutCreateWindow(title.c_str());

	glutDisplayFunc(GLApp::RenderWrapper);
	glutReshapeFunc(GLApp::OnResizeWrapper);
	glutKeyboardFunc(GLApp::OnKeyDownWrapper);
	glutSpecialFunc(GLApp::OnSpecialDownWrapper);
	glutPassiveMotionFunc(GLApp::OnMouseMoveWrapper);
}

void GLApp::OnUpdate(int val)
{
	glLoadIdentity();
	app.OnUpdate(val);
	OnRender();
	glutTimerFunc(interval, GLApp::OnUpdateWrapper, interval);
}

void GLApp::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	app.OnRender();
	glutSwapBuffers();
}

void GLApp::Run()
{
	glutTimerFunc(1, GLApp::OnUpdateWrapper, 0);
	glutMainLoop();
}

void GLApp::OnResize(Vector2f screenSize)
{
	OnResize(screenSize.x(), screenSize.y());
}

void GLApp::OnResize(int w, int h)
{
	screenSize = { float(w), float(h) };
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	app.OnResize();
}

void GLApp::OnKeyDown(int key, int x, int y)
{
	app.OnKey(key, x, y);
}

void GLApp::OnMouseMove(int x, int y)
{
	app.OnMouseMove(x, y);
}

// Static function to call the member function
void GLApp::RenderWrapper()
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnRender();
	}
}

void GLApp::OnResizeWrapper(int w, int h)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnResize(w, h);
	}
}

void GLApp::OnUpdateWrapper(int val)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnUpdate(val);
	}
}

void GLApp::OnKeyDownWrapper(unsigned char key, int x, int y)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnKeyDown(KeyCode(key), x, y);
	}
}

void GLApp::OnSpecialDownWrapper(int key, int x, int y)
{
  GLApp* app = static_cast<GLApp*>(GetInstance());
  if (app)
  {
	app->OnKeyDown(KeyCode(key), x, y);
  }
}

void GLApp::OnMouseMoveWrapper(int x, int y)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnMouseMove(x, y);
	}
}