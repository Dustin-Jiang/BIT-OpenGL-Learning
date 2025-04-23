#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"
#include "Planet.h"
#include "Euler.h"

#include <memory> // 添加内存库支持智能指针

// 使用前向声明代替包含整个头文件
class Spaceman;

class Spaceship : public Bindable
{
private:
    Vector3f position, front, up;

    Cube cube;
    Triangle wing;


    double pitch = 0.0;
    std::shared_ptr<Planet> target = nullptr;
    std::shared_ptr<Spaceman> spaceman = nullptr;

protected:
    void OnUpdate(int val) override;

public:
    Matrix4f Rotation;

    float speed = 0.5f;
    Spaceship(Vector3f pos);
    void OnDraw() override;

    void Yaw(float deg) override;
    void Pitch(float deg) override;
    void Roll(float deg) override {};

    void Follow(std::shared_ptr<Planet>& planet);

    void SetSpaceman(std::shared_ptr<Spaceman>& pSpaceman);

    Vector3f Front() override;
    Vector3f Position() override;
    Vector3f Up() override;

    void Move(Vector3f v) override {};
};