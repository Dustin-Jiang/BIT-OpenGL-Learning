#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "glut.h"
#include "Drawable.h"

#include <memory>
#include <vector>
#include "Matrix.h"

const double PI = asin(1) * 2;

enum LINE_TYPE
{
	Lines = GL_LINES,
	LineStrip = GL_LINE_STRIP,
	LineLoop = GL_LINE_LOOP
};

class Vertex3f
{
public:
	Vector3f color;
	Vector3f pos;
	Vertex3f(float x, float y, float z, Vector3f color) : pos(Vector3f(x, y, z)), color(color) {};
	Vertex3f(Vector3f v, Vector3f color) : pos(Vector3f(v)), color(color) {};

	void SetColor(Vector3f color)
	{
		this->color = color;
	}

	float& x() { return pos.x(); }
	float& y() { return pos.y(); }
	float& z() { return pos.z(); }
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
    bool isWire = false;
	std::vector<std::vector<Vector3f>> vertices;
	Sphere(Vertex3f vertex, float radius, unsigned int slices, unsigned int stacks);
	void OnDraw() override;
};

class Cylinder : public Drawable
{
public:
	float radius;
	unsigned int slices;
	Vertex3f bottom, to, top;
	std::vector<Vertex3f> vertices;

	Cylinder(Vertex3f bottom, Vertex3f to, float radius, unsigned int slices);
	void OnDraw() override;
	void OnUpdate(int val);
};

class Arrow : public Drawable
{
	Vector3f rotationAxis = { 0,0,0 };
	float rotationDegree = 0.0f;
public:
	Line line;
	Cylinder cylinder;
	Vertex3f from, direction;
	Arrow(Vertex3f from, Vertex3f direction);

	void OnDraw() override;
	void OnUpdate(int val);

	void SetPosition(Vector3f pos);
	void SetDirection(Vector3f dir);
};

Matrix4f RotationMatrix(Vector3f axis, float deg);