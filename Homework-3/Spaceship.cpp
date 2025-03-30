#include "Spaceship.h"

Spaceship::Spaceship(Vector3f pos) : cube(Cube(Vertex3f{ pos, { 0,1,0 } }, 1.0, { 0,1,0 }, { 0.6,0,0.8 })),
arrow(Arrow{ {{0,0,0}, {0,1,0}}, {{6,0,8}, {0,1,0}} }) {};

void Spaceship::OnDraw()
{
    cube.Draw();
    //arrow.Draw();
}

void Spaceship::OnUpdate(int val)
{
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