#pragma execution_character_set("uft-8")
#pragma once
#include "stdafx.h"
#include "GLApp.h"

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
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	Vector4f amb = { 0.2f,0.2f,0.2f,1 };
	Vector4f dif = { 0.3, 0.3, 0.3, 1 };
	Vector4f spe = { 0.2f,0.2f,0.2f,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.09f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.032f);
	glEnable(GL_LIGHT0);

	glutCreateWindow(title.c_str());

	glutDisplayFunc(GLApp::RenderWrapper);
	glutReshapeFunc(GLApp::OnResizeWrapper);
	glutKeyboardFunc(GLApp::OnKeyDownWrapper);
	glutKeyboardUpFunc(GLApp::OnKeyUpWrapper);  // 添加按键抬起的回调
	glutSpecialFunc(GLApp::OnSpecialDownWrapper);
	glutSpecialUpFunc(GLApp::OnSpecialUpWrapper); // 添加特殊键抬起的回调
	glutMouseFunc(GLApp::OnMouseWrapper);
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

void GLApp::OnKeyDown(Key key, int x, int y)
{
	pKeyboard->OnKeyDown(key, x, y);
}

void GLApp::OnKeyUp(Key key, int x, int y)
{
	pKeyboard->OnKeyUp(key, x, y);
}

void GLApp::OnMouseMove(int x, int y)
{
	app.OnMouseMove(x, y);
}

void GLApp::OnMouse(int button, int state, int x, int y)
{
	app.OnMouse(button, state, x, y);
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

void GLApp::OnKeyUpWrapper(unsigned char key, int x, int y)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnKeyUp(KeyCode(key), x, y);
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

void GLApp::OnSpecialUpWrapper(int key, int x, int y)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnKeyUp(KeyCode(key), x, y);
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

void GLApp::OnMouseWrapper(int button, int state, int x, int y)
{
	GLApp* app = static_cast<GLApp*>(GetInstance());
	if (app)
	{
		app->OnMouse(button, state, x, y);
	}
}