#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"

class Planet : public Drawable
{
public:
  Vector3f pos;
  float radius;
  bool isWire = false;
  Sphere sphere;
  Matrix4f spin, revolution, spinSum, revolutionSum;
  Track track;

  Planet(Vector3f pos, Vector3f color, float radius, float spin = 0.0f, float revolution = 0.0f) : pos(pos), radius(radius),
    sphere(Sphere(Vertex3f{ {0, 0, 0}, color }, radius, 16, 16)), spin(Matrix4f::Identity()), revolution(Matrix4f::Identity()),
    spinSum(Matrix4f::Identity()), revolutionSum(Matrix4f::Identity()), track(1000, 1.0f)
  {
    if (spin != 0.0f)
    {
      this->spin = RotationMatrix({ 0, 1, 0 }, spin / 30.0f);
    }
    if (revolution != 0.0f)
    {
      this->revolution = RotationMatrix({ 0, 1, 0 }, revolution / 30.0f);
    }
  };

  void OnDraw() override
  {
    track.Draw();

    glMultMatrixf(revolutionSum.getGlMatrix().data());
    glPushMatrix();

    glTranslatef(pos.x(), pos.y(), pos.z());
    glMultMatrixf(spinSum.getGlMatrix().data());

    sphere.Draw();

    glPopMatrix();
  }

  void OnUpdate(int interval) override
  {
    sphere.isWire = isWire;

    spinSum = spinSum * spin;
    revolutionSum = revolutionSum * revolution;

    track.AddPoint(Vertex3f{ WorldPosition(), {0.8, 0.8, 0.8} });
  }

  Vector3f WorldPosition()
  {
    return RotatedPosition(pos, revolutionSum);
  }
};

class Sun : public Planet
{
public:
  Sun(Vector3f pos) : Planet(pos, { 1, 0.8, 0 }, 20.0f, 0.0f, 0.5f) {};
};

class Mercury : public Planet
{
public:
  Mercury(Vector3f pos) : Planet(pos, { 0.5, 0.5, 0.5 }, 1.0f, 1.0f, 0.5f) {};
};

class Venus : public Planet
{
public:
  Venus(Vector3f pos) : Planet(pos, { 0.8, 0.8, 0.8 }, 1.0f, 0.5f, 0.3f) {};
};

class Earth : public Planet
{ 
public:
  Earth(Vector3f pos) : Planet(pos, { 0, 0.3, 1 }, 2.0f, 1.0f, 0.2f) {};
};

class Mars : public Planet
{
public:
  Mars(Vector3f pos) : Planet(pos, { 1, 0.4, 0 }, 1.5f, 0.5f, 0.1f) {};
};

class Jupiter : public Planet
{
public:
  Jupiter(Vector3f pos) : Planet(pos, { 181.f / 256, 162.f / 256, 119.f / 256 }, 8.0f, 0.5f, 0.05f) {};
};

class Saturn : public Planet
{
public:
  Saturn(Vector3f pos) : Planet(pos, { 213.f / 256, 181.f / 256, 122.f / 256 }, 7.0f, 0.5f, 0.03f) {};
};

class Uranus : public Planet
{
public:
  Uranus(Vector3f pos) : Planet(pos, { 0.5, 0.5, 1 }, 3.0f, 0.5f, 0.02f) {};
};

class Neptune : public Planet
{
public:
  Neptune(Vector3f pos) : Planet(pos, { 0, 0, 1 }, 3.0f, 0.5f, 0.01f) {};
};