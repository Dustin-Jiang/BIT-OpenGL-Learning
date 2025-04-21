#pragma once
#include "stdafx.h"
#include <cmath>
#include <iostream>

#include "Vector.h"
#include "Matrix.h"

class Quaternion; // 前向声明

#ifndef PI
#define PI 3.14159265358979323846
#endif

class Euler {
public:
    // 以弧度为单位
    float yaw;
    float pitch;
    float roll;

public:
    Euler() : yaw(0.0f), pitch(0.0f), roll(0.0f) {}

    Euler(float yaw_deg, float pitch_deg, float roll_deg, bool isDegree = false);
    Euler(const Euler& e) : pitch(e.pitch), yaw(e.yaw), roll(e.roll) {}
    Euler(const Vector3f& v);
    Euler(const Matrix4f& m);

    float h() const { return yaw; }
    float p() const { return pitch; }
    float b() const { return roll; }

    Euler Normalize() const;

    // 转换为四元数
    Quaternion ToQuaternion() const;
    Vector3f ToVector() const;
    Matrix4f ToRotateMatrix() const;

    friend std::ostream& operator<<(std::ostream& os, Euler& e) {
        os << e.h() << "," << e.p() << "," << e.b();
        return os;
    }

    Vector3f ToDegree() const;
};