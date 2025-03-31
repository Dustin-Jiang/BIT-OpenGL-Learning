#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "glut.h"
#include "Drawable.h"

#include <memory>
#include <vector>
#include <cmath>
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

class Cube : public Drawable
{
    Matrix4f rotation = Matrix4f::Identity();
public:
    std::vector<Vertex3f> vertices;
    float size;
    Vertex3f vertex;
    bool isWire = false;
    Vector3f up, right, front;
    Cube(Vertex3f vertex, float size, Vector3f up, Vector3f front);
    void OnDraw() override;
    void OnUpdate(int interval);
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

class Ring : public Drawable
{
public:
    float innerRadius, outerRadius;
    unsigned int slices;
    Vector3f pos, up, color;
    std::array<std::vector<Vector3f>, 2> vertices;
    Matrix4f rotation = Matrix4f::Identity();
    bool isWire = false;

    Ring(Vertex3f vertex, Vector3f up, float innerRadius, float outerRadius, unsigned int slices);
    void OnDraw() override;
    void OnUpdate(int val);
};

class Track : public Drawable
{
public:
    std::vector<Point> points;
    float width;
    size_t size;
    size_t current = 0;
    size_t sampling = 60, count = 0;
    Track(size_t size, float width);
    void OnDraw() override;
    void OnUpdate(int val) override;
    void AddPoint(Vertex3f point);
};

Matrix4f RotationMatrix(Vector3f axis, float deg);
Vector3f RotatedPosition(Vector3f& pos, Matrix4f& m);