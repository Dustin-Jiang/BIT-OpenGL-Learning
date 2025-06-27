#include "Spaceship.h"
#include "Spaceman.h"

Spaceship::Spaceship(Vector3f pos) : position(pos), up({ 0, 1, 0 }), front({ -0.6, 0, -0.8 }),
    cube(Cube(Vertex3f{ {0, 0, 0}, {0.65,0.65,0.65} }, 12.0, 6.0, 4.0, { 0,1,0 }, { 0, 0, -1 })), Rotation(Matrix4f::Identity()),
    wing(Triangle(Vertex3f({ 0,-2,6 }, { 0.4,0.4,0.4 }), 12.f, 24.f, { 0,0,-1 }, { 0,1,0 })),
    tail(Triangle(Vertex3f({ 0,2,5 }, { 0.4,0.4,0.4 }), 4.f, 6.f, { 0,1,0 }, { 0,0,1 }))
{
    auto steel = std::make_shared<Texture>(L"steel.bmp");
    cube.BindTexture(steel);
    wing.BindTexture(steel);
    tail.BindTexture(steel);
};

void Spaceship::OnDraw()
{
    glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glMultMatrixf(Rotation.getGlMatrix().data());
    cube.Draw();
    wing.Draw();
    tail.Draw();

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
        Vector3f right = front.Cross(worldUp).Normalized();
        up = right.Cross(front).Normalized();
    }

    if (spaceman) spaceman->Update(val);

    position += front * speed * val * 1.0f / 30.0f;
    cube.OnUpdate(val);

    Rotation = Euler(up, front).ToRotateMatrix();
}

void Spaceship::Yaw(float rad)
{
    Matrix4f rot_mat = RotationMatrix(up, rad * 180.0f / PI);
    Vector4f res = (rot_mat * Matrix4f::VMatrix(Vector4f { front.x(), front.y(), front.z(), 0.0f })).ToVector().Normalized();
    front = { res.x(), res.y(), res.z() };
    std::cout << "spaceship euler: " << Euler(up, front) << std::endl;
}

void Spaceship::Pitch(float rad)
{
    Vector3f right = front.Cross(up).Normalized();
    Matrix4f rot_mat = RotationMatrix(right, rad * 180.0f / PI);
    Vector4f res;
    res = (rot_mat * Matrix4f::VMatrix(Vector4f{ front.x(), front.y(), front.z(), 0.0f })).ToVector().Normalized();
    front = {res.x(), res.y(), res.z()};
    res = (rot_mat * Matrix4f::VMatrix(Vector4f{ up.x(), up.y(), up.z(), 0.0f })).ToVector().Normalized();
    up = { res.x(), res.y(), res.z() };
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