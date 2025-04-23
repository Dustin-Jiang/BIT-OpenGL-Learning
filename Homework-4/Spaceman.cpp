#include "Spaceship.h"
#include "Spaceman.h"

Spaceman::Spaceman(Vector3f pos, Vector3f front, float height, std::shared_ptr<Spaceship> ship) : position(pos), front(front), up({ 0, 1, 0 }),
	body(Cube(Vertex3f{ {0, 0, 0}, {0.2,0.2,0.8} }, height * 0.05, height * 0.1, height * 0.4, { 0,1,0 }, { 0, 0, -1 })),
	leftArm(Cube(Vertex3f{ {0, 0, 0}, {237.0f / 256.0f, 227.0f / 256.0f, 197.0f / 256.0f} }, height * 0.01, height * 0.01, height * 0.2, { 0,1,0 }, { 0, 0, -1 })),
	rightArm(Cube(Vertex3f{ {0, 0, 0}, {237.0f / 256.0f, 227.0f / 256.0f, 197.0f / 256.0f} }, height * 0.01, height * 0.01, height * 0.2, { 0,1,0 }, { 0, 0, -1 })),
	leftLeg(Cube(Vertex3f{ {0.0f, height * -0.2f, 0.0f}, {237.0f / 256.0f, 227.0f / 256.0f, 197.0f / 256.0f} }, height * 0.01, height * 0.05, height * 0.4, { 0,1,0 }, { 0, 0, -1 })),
	rightLeg(Cube(Vertex3f{ {0.0f, height * -0.2f, 0.0f}, {237.0f / 256.0f, 227.0f / 256.0f, 197.0f / 256.0f} }, height * 0.01, height * 0.05, height * 0.4, { 0,1,0 }, { 0, 0, -1 })),
	head(Circle(Vertex3f{ {0, 0, 0}, {237.0f / 256.0f, 227.0f / 256.0f, 197.0f / 256.0f} }, height * 0.1f, 20)),
	Rotation(Matrix4f::Identity()),
    HeadRotation(Matrix4f::Identity()),
	height(height),
	pShip(ship)
{
}

void Spaceman::OnDraw()
{
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());

	glPushMatrix();
	glMultMatrixf(Rotation.getGlMatrix().data());

	glTranslatef(0, height * 0.4, 0);

    glPushMatrix();

	glPushMatrix();
	glTranslatef(height * -0.05, 0, 0);
	glMultMatrixf(RotationMatrix({-1,0,0}, phi, false).getGlMatrix().data());
	leftLeg.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(height * 0.05, 0, 0);
	glMultMatrixf(RotationMatrix({ 1,0,0 }, phi, false).getGlMatrix().data());
	rightLeg.Draw();
	glPopMatrix();

    glPopMatrix();

	glTranslatef(height * -0.05, height * 0.4, 0);
	body.Draw();

	glTranslatef(0, height * 0.1, 0);

	glPushMatrix();

	glPushMatrix();
	glTranslatef(height * -0.1, 0, 0);
	glMultMatrixf(RotationMatrix({ -1,0,0 }, phi, false).getGlMatrix().data());
	leftArm.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(height * 0.2, 0, 0);
	glMultMatrixf(RotationMatrix({ 1,0,0 }, phi, false).getGlMatrix().data());
	rightArm.Draw();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
    glTranslatef(0, height * 0.9, 0);
	glMultMatrixf(RotationMatrix({1, 0, 0}, 90).getGlMatrix().data());
	glMultMatrixf(HeadRotation.getGlMatrix().data());
	head.Draw();
	glPopMatrix();

	glPopMatrix();
}

void Spaceman::OnUpdate(int val) {
	Vector3f worldUp = { 0, 1, 0 };

	Vector3f right = front.Cross(worldUp).Normalized();
	up = right.Cross(front);
	//position += front * speed * val * 1.0f / 30.0f;

    Rotation = Euler(worldUp, worldUp.Cross(right)).ToRotateMatrix();
	HeadRotation = Euler(up, front).ToRotateMatrix();

    if (theta >= 2 * PI) {
        theta = 0;
    }
    else if (theta - PI >= -0.02 && theta < PI + 0.005f) {
        theta = PI;
    }
	else if (theta - 0.0f >= 1e-5) {
		theta += 0.01f;
	}
    phi = MAX_ANGLE * std::sin(theta);
}

Vector3f Spaceman::Position() {
	// 摄像机距离
	//Vector3f dist { 0, 0, -1 }, h { 0.f, height, 0.f };
	Vector3f dist{ 0, 0, 0 }, h{ 0.f, height, 0.f };

	Vector3f pos = position - dist + h;
	if (pShip == nullptr) {
		std::cout << "Warning: ship pointer is null in Spaceman::Position()" << std::endl;
		return pos;
	}
	return pShip->Position() + RotatedPosition(pos, pShip->Rotation);
}
Vector3f Spaceman::Up() {
	if (pShip == nullptr) {
		std::cout << "Warning: ship pointer is null in Spaceman::Up()" << std::endl;
		return up;
	}
	auto res = RotatedPosition(up, pShip->Rotation);
	if (res.Length() <= 1e-5) { res = {0,1,0}; }
	return res;
}
Vector3f Spaceman::Front() { 
	if (pShip == nullptr) {
		std::cout << "Warning: ship pointer is null in Spaceman::Front()" << std::endl;
		return front;
	}
	return RotatedPosition(front, pShip->Rotation); 
}

void Spaceman::Yaw(float rad) {
    auto rot = Matrix3f(cos(rad), 0.f, sin(rad), 0.f, 1.f, 0.f, -sin(rad), 0.f, cos(rad));
    front = (rot * Matrix3f::VMatrix(front)).ToVector().Normalized();
    up = (rot * Matrix3f::VMatrix(up)).ToVector().Normalized();
}

void Spaceman::Pitch(float rad) {
    if (abs(pitch + rad) >= std::asin(0.95))
        return;
    front = (front * cos(rad) + up * sin(rad)).Normalized();
    auto right = front.Cross({ 0, 1, 0 });
    up = right.Cross(front).Normalized();
	pitch += rad;
}

void Spaceman::Move(Vector3f v) {
    auto worldUp = Vector3f{ 0, 1, 0 };
	auto direction = worldUp.Cross((RotatedPosition(v, pShip->Rotation.Inverse())).Cross(worldUp)).Normalized();
	position += direction * 0.01f;
	auto clamp = [](float v, float from, float to) {
        if (v > to) return to;
        if (v < from) return from;
        return v;
	};
	position = { clamp(position.x(), -3.0, 3.0), position.y(), clamp(position.z(), -6.0, 6.0)};

	theta += 0.01f;
}