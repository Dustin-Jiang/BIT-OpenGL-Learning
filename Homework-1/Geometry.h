#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "glut.h"
#include "Drawable.h"

#include <memory>
#include <vector>

const double PI = asin(1) * 2;

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

	void SetColor(Vector3f color)
	{
		this->color = color;
	}
};

class Line : public Drawable
{
public:
	std::vector<Vertex3f> vertices;
	LINE_TYPE type;
	float width;

	Line(std::vector<Vertex3f> vertices, LINE_TYPE type);
	Line(std::vector<Vertex3f> vertices, LINE_TYPE type, float width);

	void SetWidth(float w);
	void OnDraw() override;
};

class Point : public Drawable
{
public:
	Vertex3f vertex;
	float size;
	Point(Vertex3f vertex, float size) : vertex(vertex), size(size) {};
	void OnDraw() override;
};

class Sphere : public Drawable
{
public:
	float radius;
	unsigned int slices, stacks;
	Vertex3f vertex;
	std::vector<std::vector<Vector3f>> vertices;
	Sphere(Vertex3f vertex, float radius, unsigned int slices, unsigned int stacks);
	void OnDraw() override;
};