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
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		OnDraw();
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

class DrawableList : public Drawable
{
protected:
	std::vector<pDrawable> objs;
public:
	DrawableList() : Drawable(), objs() {};
	void Add(pDrawable obj)
	{
		objs.push_back(obj);
	}
	void Draw() override
	{
		this->OnDraw();
		for (auto& obj : objs)
		{
			obj->Draw();
		}
	}
	void Update(int val)
	{
		this->OnUpdate(val);
		for (auto& obj : objs)
		{
			obj->Update(val);
		}
	}
};