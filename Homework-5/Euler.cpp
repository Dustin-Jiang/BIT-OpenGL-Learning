#include "Euler.h"
#include "Quaternion.h"

Euler::Euler(float yaw_deg, float pitch_deg, float roll_deg, bool isDegree) {
    if (isDegree) {
        yaw = yaw_deg * PI / 180.0f;
        pitch = pitch_deg * PI / 180.0f;
        roll = roll_deg * PI / 180.0f;
    }
    else {
        yaw = yaw_deg;
        pitch = pitch_deg;
        roll = roll_deg;
    }
}

Euler::Euler(const Vector3f& v): pitch(0.0f), yaw(0.0f), roll(0.0f) {
    Vector3f normalized = v.Normalized();

    yaw = atan2(-normalized.x(), -normalized.z());
    pitch = asin(normalized.y());
    // 滚转角设为0
    roll = 0.0f;
}

Euler::Euler(const Vector3f& up, const Vector3f& front) : pitch(0.0f), yaw(0.0f), roll(0.0f) {
    Vector3f front_n = front.Normalized();
    Vector3f up_n = up.Normalized();
    
    pitch = asin(front_n.y()); // 直接使用y分量作为sin值

    // 只有当俯仰角不接近90度时才计算偏航角
    if (abs(pitch) < PI / 2.0f - 1e-5) {
        yaw = atan2(-front_n.x(), -front_n.z());
    }
    else {
        // 当俯仰角为90度时，偏航角未定义，可以将其设置为0
        Vector3f right = Vector3f{0, 1, 0}.Cross(up_n);
        yaw = atan2(right.z(), right.x());
    }

    // 计算滚转角
    Vector3f no_roll_right = Vector3f{0, 1, 0}.Cross(front_n);

    if (no_roll_right.Length() < 1e-10) {
        roll = 0.0f;
    }
    else {
        no_roll_right = no_roll_right.Normalized();
        Vector3f no_roll_up = front_n.Cross(no_roll_right).Normalized();

        roll = acos(std::max(-1.0f, std::min(1.0f, up_n.Dot(no_roll_up))));

        // 判断滚转方向
        if (up_n.Dot(no_roll_right) < 0) {
            roll = -roll;
        }
    }
}

Euler::Euler(const Matrix4f& m) {
    pitch = asin(-m(2, 1));
    if (cos(pitch) <= 1e-6) {
        roll = 0.0f;
        yaw = atan2(-m(0, 2), m(0, 0));
    }
    else {
        yaw = atan2(m(2, 0), m(2, 2));
        roll = atan2(m(0, 1), m(1, 1));
    }
}

Euler Euler::Normalize() const {
    auto normalize = [](float f) {
        while (f >= PI) {
            f -= 2 * PI;
        }
        while (f <= -PI) {
            f += 2 * PI;
        }
        return f;
    };
    return Euler { normalize(yaw), normalize(pitch), normalize(roll) };
}

Quaternion Euler::ToQuaternion() const {
    Quaternion qh { cos(h() / 2), 0, sin(h() / 2), 0};
    Quaternion qp { cos(p() / 2), sin(p() / 2), 0, 0 };
    Quaternion qb { cos(b() / 2), 0, 0, sin(b() / 2) };
    return qh * qp * qb;
}

Vector3f Euler::ToVector() const {
    return Vector3f { -sin(yaw) * cos(pitch), sin(pitch), -cos(yaw) * cos(pitch)};
}

Vector3f Euler::FrontVector() const {
    return ToVector().Normalized();
}

Vector3f Euler::UpVector() const {
    auto rotMatrix = ToRotateMatrix();
    auto worldUp = Vector3f{ 0, 1, 0 };
    Matrix3f rotation(
        rotMatrix(0, 0), rotMatrix(0, 1), rotMatrix(0, 2),
        rotMatrix(1, 0), rotMatrix(1, 1), rotMatrix(1, 2),
        rotMatrix(2, 0), rotMatrix(2, 1), rotMatrix(2, 2)
    );
    return (rotation * Matrix3f::VMatrix(worldUp)).ToVector().Normalized();
}

Matrix4f Euler::ToRotateMatrix() const {
    // 按照 roll -> pitch -> yaw 的顺序合成旋转矩阵
    return (
        RotationMatrix({ 0,1,0 }, yaw, false) *
        RotationMatrix({ 1,0,0 }, pitch, false) *
        RotationMatrix({ 0,0,1 }, roll, false)
    );
}

Vector3f Euler::ToDegree() const {
    float y = yaw / PI * 180.0f, p = pitch / PI * 180.0f, r = roll / PI * 180.0f;
    return Vector3f { y, p, r };
}