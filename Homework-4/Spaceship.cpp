#include "Spaceship.h"
#include "Spaceman.h"

Spaceship::Spaceship(Vector3f pos) : position(pos), up({ 0, 1, 0 }), front({ 0.6, 0, 0.8 }),
    cube(Cube(Vertex3f{ {0, 0, 0}, {0.65,0.65,0.65} }, 12.0, 6.0, 4.0, { 0,1,0 }, { 0, 0, -1 })), Rotation(Matrix4f::Identity()),
    wing(Triangle(Vertex3f({ 0,-2,6 }, { 0.65,0.65,0.65 }), 12.f, 24.f, { 0,0,-1 }, { 0,1,0 }))
{
};

void Spaceship::OnDraw()
{
    glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glMultMatrixf(Rotation.getGlMatrix().data());
    cube.Draw();
    wing.Draw();

    // 通过指针调用方法
    if (spaceman) {
        spaceman->Draw();
    }

    glPopMatrix();
}

void Spaceship::OnUpdate(int val)
{
    Vector3f worldUp = { 0, 1, 0 };

    if (target)
    {
        front = (target->WorldPosition() - position).Normalized();
        auto dist = (target->WorldPosition() - position).Length();
        if (dist <= target->radius + 0.5f)
        {
            target = nullptr;
        }
    }

    if (spaceman) spaceman->Update(val);

    Vector3f right = front.Cross(worldUp).Normalized();
    up = right.Cross(front);
    position += front * speed * val * 1.0f / 30.0f;
    cube.OnUpdate(val);

    Rotation = Euler(up, front).ToRotateMatrix();
}

void Spaceship::Yaw(float rad)
{
    auto rot = Matrix3f(cos(rad), 0.f, sin(rad), 0.f, 1.f, 0.f, -sin(rad), 0.f, cos(rad));
    front = (rot * Matrix3f::VMatrix(front)).ToVector().Normalized();
    up = (rot * Matrix3f::VMatrix(up)).ToVector().Normalized();
}

void Spaceship::Pitch(float rad)
{
    if (abs(pitch + rad) >= std::asin(0.75))
        return;
    front = (front * cos(rad) + up * sin(rad)).Normalized();
    auto right = front.Cross({ 0, 1, 0 });
    up = right.Cross(front).Normalized();
    pitch += rad;
}

void Spaceship::Follow(std::shared_ptr<Planet>& p)
{
    target = p;
}

void Spaceship::SetSpaceman(std::shared_ptr<Spaceman>& pSpaceman)
{
    spaceman = pSpaceman;
}

Vector3f Spaceship::Position() {
    return position;
}

Vector3f Spaceship::Up() {
    return up;
}

Vector3f Spaceship::Front() {
    return front;
}