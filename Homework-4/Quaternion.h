#pragma once
#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <array>

#include "Vector.h"
#include "Matrix.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

class Euler;

class Quaternion {
public:
    // 四元数分量
    float w, x, y, z;

public:
    Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

    Quaternion(float _w, float _x, float _y, float _z)
        : w(_w), x(_x), y(_y), z(_z) {
    }

    Quaternion(const Matrix4f& m);

    Quaternion(float radius, const Vector3f& axis, bool isDegree = false) {
        if (isDegree) radius = radius / 180.0f * PI;
        float half_angle = radius * 0.5f;
        float sin_half_angle = std::sin(half_angle);
        w = std::cos(half_angle);
        x = axis.x() * sin_half_angle;
        y = axis.y() * sin_half_angle;
        z = axis.z() * sin_half_angle;
    }

    Quaternion& operator=(const Quaternion& q) {
        if (this != &q) {
            w = q.w;
            x = q.x;
            y = q.y;
            z = q.z;
        }
        return *this;
    }

    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }

    Quaternion operator-(const Quaternion& q) const {
        return q * (*this).Inversed();
    }

    Quaternion operator-() const {
        return Quaternion(-w, -x, -y, -z);
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar).Normalized();
    }

    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    float Dot(const Quaternion& q) const {
        return w * q.w + x * q.x + y * q.y + z * q.z;
    }

    float Length() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    void Inverse();
    Quaternion Inversed() const;

    Quaternion Normalized() const;

    Quaternion Pow(float t) const;

    float GetAngle(bool isDegree = false) const;
    Vector3f GetAxis() const;

    Quaternion Slerp(const Quaternion& q, float t) const;
    Quaternion QSlerp(const Quaternion& q, float t) const;

    template <size_t SIZE>
    std::array<Quaternion, SIZE> Slerp(const std::array<Quaternion, SIZE>& q, float t) const;

    Euler ToEuler() const;

    Matrix4f ToRotateMatrix() const;

    friend std::ostream& operator<<(std::ostream& os, Quaternion& q) {
        os << q.w << "," << q.x << "," << q.y << "," << q.z;
        return os;
    }
};