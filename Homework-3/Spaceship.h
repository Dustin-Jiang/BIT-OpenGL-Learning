#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"

class Spaceship : public Drawable
{
private:
    Cube cube;
    Arrow arrow;

public:
    float speed = 0.5f;
    Spaceship(Vector3f pos);
    void OnDraw() override;
    void OnUpdate(int val);

    void Yaw(float deg);
};