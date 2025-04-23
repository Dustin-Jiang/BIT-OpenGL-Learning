#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Matrix.h"
#include "Geometry.h"

class Spaceship;

class Spaceman : public Bindable
{
private:
    float theta = 0.0f, phi = 0.0f;
    const float MAX_ANGLE = PI / 4.0f;

public:
    Vector3f position, front, up;
    Matrix4f Rotation, HeadRotation;
    Cube body, leftArm, rightArm, leftLeg, rightLeg;
    Circle head;
    float height;

    double pitch = 0.0;

    std::shared_ptr<Spaceship> pShip = nullptr;

    Spaceman(Vector3f pos, Vector3f front, float height, std::shared_ptr<Spaceship> ship);
    void OnDraw() override;
    void OnUpdate(int val) override;

    Vector3f Position() override;
    Vector3f Up() override;
    Vector3f Front() override;

    void Yaw(float deg) override;
    void Pitch(float deg) override;
    void Roll(float deg) override {};

    void Move(Vector3f v) override;
};