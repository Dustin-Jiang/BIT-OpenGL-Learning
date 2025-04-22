#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"
#include "Planet.h"

class Spaceship : public Bindable
{
private:
    Cube cube;
    double pitch = 0.0;
    std::shared_ptr<Planet> target = nullptr;

protected:
    void OnUpdate(int val) override;

public:
    float speed = 0.5f;
    Spaceship(Vector3f pos);
    void OnDraw() override;

    void Yaw(float deg);
    void Pitch(double deg);
    void Follow(std::shared_ptr<Planet>& planet);

    Vector3f Front() override;
    Vector3f Position() override;
    Vector3f Up() override;
};