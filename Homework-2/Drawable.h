#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "glut.h"

#include <vector>
#include <memory>

class Drawable
{
public:
	virtual void Draw()
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		OnDraw();
		glPopMatrix();
		glPopAttrib();
	};
	void Update(int val)
	{
		OnUpdate(val);
	}
protected:
	virtual void OnDraw() = 0;
	virtual void OnUpdate(int val) {};
};

using pDrawable = std::shared_ptr<Drawable>;