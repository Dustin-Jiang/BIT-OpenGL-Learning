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
    if (fabs(up.Dot(front)) > 1e-5) {
        std::cerr << "Euler: up and front vectors are not orthogonal! Dot Value: " << up.Dot(front) << std::endl;
        return;
    }
    auto worldUp = Vector3f{ 0, 1, 0 };
    Vector3f normalized = front.Normalized();
    Vector3f upNormalized = up.Normalized();
    yaw = atan2(-normalized.x(), -normalized.z());

    pitch = asin(normalized.y());
    
    // 计算滚转角
    Vector3f right = front.Cross(worldUp).Normalized();
    Vector3f noRollUp = right.Cross(front).Normalized();

    auto cosRoll = upNormalized.Dot(noRollUp);
    if (fabs(fabs(cosRoll) - 1.0f) < 1e-6) {
        roll = 0.0f;
    }
    else {
        roll = acos(cosRoll);
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