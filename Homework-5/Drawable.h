#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "glut.h"

#include <vector>
#include <memory>
#include "Texture.h"

class Drawable
{
public:
	std::shared_ptr<Texture> texture = nullptr;
	virtual void Draw()
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_NORMALIZE);
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
	};
	void BindTexture(std::shared_ptr<Texture> t) {
		texture = t;
	}
protected:
	virtual void OnDraw() {};
	virtual void OnUpdate(int val) {};
};

using pDrawable = std::shared_ptr<Drawable>;

class Bindable : public Drawable {
public:
	virtual Vector3f Position() = 0;
	virtual Vector3f Front() = 0;
	virtual Vector3f Up() = 0;
    virtual Vector3f Right() {
        return Front().Cross(Up());
    }

    virtual void Yaw(float rad) = 0;
    virtual void Pitch(float rad) = 0;
    virtual void Roll(float rad) = 0;

	virtual void Move(Vector3f v) = 0;
};