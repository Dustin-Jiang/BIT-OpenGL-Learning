#pragma once
#include "stdafx.h"

#include "Geometry.h"
#include "Matrix.h"

#include <cmath>
#include <cassert>

const auto worldUp = Vector3f{ 0, 1, 0 };

class Camera
{
protected:
    float pitch = 0.0f;
public:
    Vector3f Up, Right, Front;
    Vector3f Position;
    Camera() = default;
    virtual void Move(Vector3f v) = 0;
    virtual void Yaw(float rad) = 0;
    virtual void Pitch(float rad) = 0;
    virtual void Roll(float rad) = 0;
    virtual Vector3f Center() = 0;
    virtual void SetPosition(Vector3f pos) = 0;
    virtual void SetLookAt(Vector3f lookat) = 0;
};

class VectorCamera : public Camera
{
public:
    VectorCamera(Vector3f position = { 0, 0, 0 }, Vector3f up = { 0, 1, 0 }, Vector3f front = { 0, 0, -1 }) 
    {
        Position = position;
        Up = up;
        Front = front;
        Right = Front.Cross(Up);
    }

    void Move(Vector3f v)
    {
        Position += v;
    };

    void Yaw(float rad)
    {
        auto rot = Matrix3f(cos(rad), 0.f, sin(rad), 0.f, 1.f, 0.f, -sin(rad), 0.f, cos(rad));
        Front = (rot * Matrix3f::VMatrix(Front)).ToVector().Normalized();
        Right = (rot * Matrix3f::VMatrix(Right)).ToVector().Normalized();
        Up = (rot * Matrix3f::VMatrix(Up)).ToVector().Normalized();
    }

    void Pitch(float rad)
    {
        if (abs(pitch + rad) > std::asin(0.95))
            return;
        Front = (Front * cos(rad) + Up * sin(rad)).Normalized();
        Up = Right.Cross(Front).Normalized();
        pitch += rad;
    }

    void Roll(float rad)
    {
        auto rot = Matrix3f(cos(rad), -sin(rad), 0.f, sin(rad), cos(rad), 0.f, 0.f, 0.f, 1.f);
        Right = (rot * Matrix3f::VMatrix(Right)).ToVector().Normalized();
        Up = (rot * Matrix3f::VMatrix(Up)).ToVector().Normalized();
    }

    Vector3f Center()
    {
        return Position + Front;
    }

    void SetPosition(Vector3f pos)
    {
        Position = pos;
    }

    void SetLookAt(Vector3f lookat)
    {
        Front = (lookat - Position).Normalized();
        Right = Front.Cross(worldUp).Normalized();
        Up = Right.Cross(Front);
    }
};