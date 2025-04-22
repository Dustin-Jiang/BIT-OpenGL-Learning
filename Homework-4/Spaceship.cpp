#include "Spaceship.h"

Spaceship::Spaceship(Vector3f pos) :
    cube(Cube(Vertex3f{ pos, { 0,1,0 } }, 1.0, { 0,1,0 }, { 0.6,0,0.8 })) {};

void Spaceship::OnDraw()
{
    cube.Draw();
}

void Spaceship::OnUpdate(int val)
{
    Vector3f worldUp = { 0, 1, 0 };

    if (target)
    {
        cube.front = (target->WorldPosition() - cube.vertex.pos).Normalized();
        auto dist = (target->WorldPosition() - cube.vertex.pos).Length();
        if (dist <= target->radius + 0.5f)
        {
            target = nullptr;
        }
    }

    cube.right = cube.front.Cross(worldUp).Normalized();
    cube.up = cube.right.Cross(cube.front);
    cube.vertex.pos += cube.front * speed * val * 1.0f / 30.0f;
    cube.OnUpdate(val);
}

void Spaceship::Yaw(float rad)
{
    auto rot = Matrix3f(cos(rad), 0.f, sin(rad), 0.f, 1.f, 0.f, -sin(rad), 0.f, cos(rad));
    cube.front = (rot * Matrix3f::VMatrix(cube.front)).ToVector().Normalized();
    cube.right = (rot * Matrix3f::VMatrix(cube.right)).ToVector().Normalized();
    cube.up = (rot * Matrix3f::VMatrix(cube.up)).ToVector().Normalized();
}

void Spaceship::Pitch(double rad)
{
    if (abs(pitch + rad) >= std::asin(0.75))
        return;
    cube.front = (cube.front * cos(rad) + cube.up * sin(rad)).Normalized();
    cube.up = cube.right.Cross(cube.front).Normalized();
    pitch += rad;
}

void Spaceship::Follow(std::shared_ptr<Planet> &p)
{
    target = p;
}

Vector3f Spaceship::Position() {
    return cube.vertex.pos;
}

Vector3f Spaceship::Up() {
    return cube.up;
}

Vector3f Spaceship::Front() {
    return cube.front;
}