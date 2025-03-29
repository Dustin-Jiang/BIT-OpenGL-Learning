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


  Planet(Vector3f pos, Vector3f color, float radius) : pos(pos), radius(radius), sphere(Sphere(Vertex3f{ {0, 0, 0}, color }, radius, 50, 50)) {};

  void OnDraw() override
  {
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());

    sphere.Draw();

    glPopMatrix();
  }

  void OnUpdate(int interval) override
  {
    sphere.isWire = isWire;
  }
};

class Sun : public Planet
{
public:
  Sun(Vector3f pos) : Planet(pos, { 1, 0.5, 0 }, 10.0f) {};
};