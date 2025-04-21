#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"
#include "Planet.h"

class Spaceship : public Drawable
{
private:
    Cube cube;
    double pitch = 0.0;
    std::shared_ptr<Planet> target = nullptr;

public:
    float speed = 0.5f;
    Spaceship(Vector3f pos);
    void OnDraw() override;
    void OnUpdate(int val);

    void Yaw(float deg);
    void Pitch(double deg);
    void Follow(std::shared_ptr<Planet>& planet);
};