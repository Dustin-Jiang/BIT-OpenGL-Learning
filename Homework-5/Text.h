#pragma once
#include "stdafx.h"

#include "glut.h"
#include "Drawable.h"
#include <string>

class Text : public Drawable {
private: 
	std::string content;
	Vector2f pos;
public:
	Text(std::string text, Vector2f pos): content(text), pos(pos) {}

	void OnDraw() override {
		// 设置投影方式：平行投影
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

		glDisable(GL_LIGHTING);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// 输出字符串
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(pos.x(), pos.y());

		for (auto c : content) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
		}

		glEnable(GL_LIGHTING);

		// 恢复投影方式
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void SetContent(std::string text) {
		content = text;
	}

	void Clear() {
		content = "";
	}
};