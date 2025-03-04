#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "glut.h"

#include <vector>
#include <memory>

class Drawable
{
public:
	void Draw()
	{
		OnDraw();
	};
protected:
	virtual void OnDraw() = 0;
};

using pDrawable = std::shared_ptr<Drawable>;

enum LINE_TYPE
{
	Lines = GL_LINES,
	LineStrip = GL_LINE_STRIP,
	LineLoop = GL_LINE_LOOP
};

//using Vertex3f = Vector3f;
class Vertex3f: public Vector3f
{
public:
	Vector3f color;
	Vertex3f(float x, float y, float z, Vector3f color) : Vector3f(x, y, z), color(color) {};
	Vertex3f(Vector3f v, Vector3f color) : Vector3f(v), color(color) {};
};

class Line : public Drawable
{
public:
	const std::vector<Vertex3f> vertices;
	LINE_TYPE type;
	float width;

	Line(std::vector<Vertex3f> vertices, LINE_TYPE type);
	Line(std::vector<Vertex3f> vertices, LINE_TYPE type, float width);

	void SetWidth(float w);
	void OnDraw() override;
};