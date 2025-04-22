#include "stdafx.h"
#include "Euler.h"
#include "Quaternion.h"

Quaternion::Quaternion(const Matrix4f& m) {
    w = std::sqrt(m(0,0) + m(1,1) + m(2,2) + 1) / 2.0f;
    float s = 1 / (4.0f * w);
    x = (m(1,2) - m(2,1)) * s;
    y = (m(2,0) - m(0,2)) * s;
    z = (m(0,1) - m(1,0)) * s;
}

void Quaternion::Inverse() {
    float norm = w * w + x * x + y * y + z * z;
    if (norm > 0.0f) {
        w = w / norm;
        x = -x / norm;
        y = -y / norm;
        z = -z / norm;
    }
}

Quaternion Quaternion::Inversed() const {
    float norm = w * w + x * x + y * y + z * z;
    if (norm > 0.0f) {
        return Quaternion(w / norm, -x / norm, -y / norm, -z / norm);
    }
    return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); // 返回零四元数
}

Quaternion Quaternion::Normalized() const {
    float norm = std::sqrt(w * w + x * x + y * y + z * z);
    if (norm < 1e-6) {
        std::cerr << "警告：四元数模长接近零！";
        norm = 1.f;
    }
    return Quaternion { w / norm, x / norm, y / norm, z / norm };
}

Quaternion Quaternion::Pow(float t) const {
    if (std::abs(t) < 1e-6f) {
        return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // 任何四元数的0次幂都是单位四元数
    }
    if (std::abs(t - 1.0f) < 1e-6f) {
        return *this; // 1次幂就是四元数本身
    }

    float norm = std::sqrt(w * w + x * x + y * y + z * z);
    if (norm < 1e-6f) {
        return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); // 零四元数的幂仍是零四元数
    }

    // 将四元数转换为角轴表示
    float angle = 2.0f * std::acos(w / norm);
    float sin_half_angle = std::sqrt(1.0f - (w * w) / (norm * norm));
    
    Vector3f axis;
    if (sin_half_angle < 1e-6f) {
        axis = Vector3f(1.0f, 0.0f, 0.0f); // 当旋转角度接近0时，选择x轴作为默认轴
    } else {
        axis = Vector3f(
            x / (norm * sin_half_angle),
            y / (norm * sin_half_angle),
            z / (norm * sin_half_angle)
        );
    }

    float new_angle = angle * t * 0.5f;
    float new_w = std::cos(new_angle);
    float new_sin = std::sin(new_angle);
    
    return Quaternion(
        new_w,
        axis.x() * new_sin,
        axis.y() * new_sin,
        axis.z() * new_sin
    ).Normalized();
}

Quaternion Quaternion::Slerp(const Quaternion& _q, float t) const {
    auto q0 = (*this).Normalized(), q1 = _q.Normalized();
    auto dq = q1 * Inversed();
    std::cout << "dq: " << dq << std::endl;
    std::cout << "dq^t: " << dq.Pow(t) << std::endl;
    auto result = dq.Pow(t) * q0;
    std::cout << "result: " << result << std::endl;
    return result;
}

Quaternion Quaternion::QSlerp(const Quaternion& _q, float t) const {
    float cos_theta = Dot(_q);
    Quaternion q = _q;

    if (cos_theta < 0.0f) {
        cos_theta = -cos_theta;
        q = -q;
    }

    if (cos_theta > 1.0f - 1e-6f) {
        return Normalized();
    }

    float theta = std::acos(cos_theta);
    float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

    if (std::abs(sin_theta) < 1e-6f) { // 角度接近 0 或 π
        return Normalized();
    }

    float inv_sin_theta = 1.0f / sin_theta;
    float a = std::sin((1.0f - t) * theta) * inv_sin_theta;
    float b = std::sin(t * theta) * inv_sin_theta;

    auto res = Quaternion(
        a * w + b * q.w, // 确保成员变量名正确
        a * x + b * q.x,
        a * y + b * q.y,
        a * z + b * q.z
    );

    return res.Normalized();
}

template <size_t SIZE>
std::array<Quaternion, SIZE> Quaternion::Slerp(const std::array<Quaternion, SIZE>& q, float t) const {
    std::array<Quaternion, SIZE> result;
    for (size_t i = 0; i < SIZE; ++i) {
        result[i] = Slerp(q[i], t);
    }
    return result;
}

float Quaternion::GetAngle(bool isDegree) const {
    float radius = 2.0f * std::acos(w);
    if (radius > PI) radius -= 2 * PI;
    if (!isDegree) return radius;
    return radius / PI * 180.0f;
}

Vector3f Quaternion::GetAxis() const {
    float sin_half_angle = std::sqrt(1.0f - w * w);
    if (sin_half_angle < 1e-6) {
        return Vector3f(1.0f, 0.0f, 0.0f); // 返回单位向量
    }
    return Vector3f(x / sin_half_angle, y / sin_half_angle, z / sin_half_angle);
}

Matrix4f Quaternion::ToRotateMatrix() const {
    Quaternion q = Normalized();
    Matrix4f v = {
        1 - 2 * y * y - 2 * z * z, 2 * x * y + 2 * w * z, 2 * x * z - 2 * w * y, 0.0f,
        2 * x * y - 2 * w * z, 1 - 2 * x * x - 2 * z * z, 2 * y * z + 2 * w * x, 0.0f,
        2 * x * z + 2 * w * y, 2 * y * z - 2 * w * x, 1 - 2 * x * x - 2 * y * y, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return v;
}

Euler Quaternion::ToEuler() const {
    float p = asin(2 * w* x-2*y*z);
    float cosp = cos(p);
    if (abs(cosp) >= 1e-6) {
        float h = atan2(2 * w * y + 2 * x * z, 1 - 2 * x * x - 2 * y * y);
        float b = atan2(2 * w * z + 2 * x * y, 1 - 2 * x * x - 2 * z * z);
        return Euler { h, p, b };
    }
    float h = atan2(2 * w * y - 2 * x * z, 1 - 2 * z * z - 2 * y * y);
    float b = 0;
    return Euler { h, p, b };
}

Vector3f Quaternion::ToVector() const {
    Quaternion direction = { 0, 1, 0, 0 };
    auto result = (*this) * direction * Inversed();
    return Vector3f { result.x, result.y, result.z };
}