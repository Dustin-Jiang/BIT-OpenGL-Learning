#pragma once
#include "stdafx.h"

#include "Geometry.h"
#include "Matrix.h"
#include "Euler.h"
#include "Drawable.h"

#include <cmath>
#include <cassert>
#include "Euler.h"
#include "Quaternion.h"

const auto worldUp = Vector3f{ 0, 1, 0 };

class Camera
{
protected:
	Vector3f up, right, front;
	Vector3f position;
	std::shared_ptr<Bindable> pTarget = nullptr;
public:
	Camera() = default;
	virtual void Move(Vector3f v) = 0;
	virtual void Yaw(float rad) = 0;
	virtual void Pitch(float rad) = 0;
	virtual void Roll(float rad) = 0;
	virtual Vector3f Center() = 0;
	virtual void SetPosition(Vector3f pos) = 0;
	virtual void SetLookAt(Vector3f lookat) = 0;
	virtual Vector3f& Up() { return pTarget ? pTarget->Up() : up; }
	virtual Vector3f& Right() { return pTarget ? pTarget->Right() : right; }
	virtual Vector3f& Front() { return pTarget ? pTarget->Front() : front; }
	virtual Vector3f& Position() {
		if (pTarget)
		{
			position = pTarget->Position();
		}
		return position;
	}
	void SetTarget(Bindable& target) {
		pTarget = std::shared_ptr<Bindable>(&target);
	}
	void ReleaseTarget(std::shared_ptr<Bindable>& dest) {
        dest = pTarget;
        pTarget = nullptr;
	}
};

class VectorCamera : public Camera
{
public:
	VectorCamera(Vector3f position = { 0, 0, 0 }, Vector3f up = { 0, 1, 0 }, Vector3f front = { 0, 0, -1 })
	{
		if (fabs(up.Dot(front)) > 1e-5) {
			std::cerr << "Euler: up and front vectors are not orthogonal! Dot Value: " << up.Dot(front) << std::endl;
			return;
		}
		Camera::position = position;
		Camera::up = up.Normalized();
		Camera::front = front.Normalized();
		Camera::right = front.Cross(up).Normalized();
	}

	void Move(Vector3f v)
	{
		Position() += v;
	};

	void Yaw(float rad)
	{
		Front() = (Front() * cos(rad) - Right() * sin(rad)).Normalized();
		Right() = Front().Cross(Up()).Normalized();
	}

	void Pitch(float rad)
	{
		Front() = (Front() * cos(rad) + Up() * sin(rad)).Normalized();
		Up() = Right().Cross(Front()).Normalized();
	}

	void Roll(float rad)
	{
		auto rot = Matrix3f(cos(rad), -sin(rad), 0.f, sin(rad), cos(rad), 0.f, 0.f, 0.f, 1.f);
		Right() = (rot * Matrix3f::VMatrix(Right())).ToVector().Normalized();
		Up() = (rot * Matrix3f::VMatrix(Up())).ToVector().Normalized();
	}

	Vector3f Center()
	{
		return Position() + Front();
	}

	void SetPosition(Vector3f pos)
	{
		Position() = pos;
	}

	void SetLookAt(Vector3f lookat)
	{
		Front() = (lookat - Position()).Normalized();
		Right() = Front().Cross(worldUp).Normalized();
		Up() = Right().Cross(Front()).Normalized();
	}
};

class EulerCamera : public Camera
{
	Euler euler;
public:
	Vector3f& Up() { up = pTarget ? pTarget->Up() : euler.UpVector(); return up; }
	Vector3f& Front() { front = pTarget ? pTarget->Front() : euler.FrontVector(); return front; }
	Vector3f& Right() { right = pTarget ? pTarget->Right() : euler.FrontVector().Cross(Up()); return right; }
	
	EulerCamera(Vector3f position = { 0, 0, 0 }, Euler euler = { 0.0f, 0.0f, 0.0f }): euler(euler)
	{
		Position() = position;
		up = euler.UpVector();
		front = euler.FrontVector();
		right = Front().Cross(Up());
		std::cout << "EulerCamera: " << euler << std::endl;
	}
	void Move(Vector3f v)
	{
		Position() += v;
	};
	void Yaw(float rad)
	{
		euler.yaw += rad;
	}
	void Pitch(float rad)
	{
		euler.pitch += rad;
	}
	void Roll(float rad)
	{
		euler.roll += rad;
	}
	Vector3f Center()
	{
		return Position() + Front();
	}
	void SetPosition(Vector3f pos)
	{
		Position() = pos;
	}
	void SetLookAt(Vector3f lookat)
	{
		front = (lookat - Position()).Normalized();
		right = front.Cross(worldUp).Normalized();
		up = right.Cross(front).Normalized();
		euler = Euler(up, front);
		std::cout << "SetLookAt: " << euler << std::endl;
	}
};

class QuaternionCamera : public Camera
{
	Quaternion q;
public:
	Vector3f& Up() { up = pTarget ? pTarget->Up() : q.ToVector().Cross(Front()); return up; }
	Vector3f& Front() { front = pTarget ? pTarget->Front() : q.ToVector().Normalized(); return front; }
	Vector3f& Right() { right = pTarget ? pTarget->Right() : front.Cross(Up()); return right; }
	QuaternionCamera(Vector3f position = { 0, 0, 0 }, Quaternion q = { 1, 0, 0, 0 }) : q(q)
	{
		Position() = position;
		auto worldUp = Vector3f { 0, 1, 0 };
		front = q.ToVector().Normalized();
		right = Front().Cross(worldUp);
		up = right.Cross(Front());
	}
	void Move(Vector3f v)
	{
		Position() += v;
	};
	void Yaw(float rad)
	{
		auto worldUp = Vector3f { 0, 1, 0 };
		q = q * Quaternion(rad, worldUp);
	}
	void Pitch(float rad)
	{
		q = q * Quaternion(rad, Right());
	}
	void Roll(float rad)
	{
		q = q * Quaternion(rad, Front());
	}
	Vector3f Center()
	{
		return Position() + Front();
	}
	void SetPosition(Vector3f pos)
	{
		Position() = pos;
	}
	void SetLookAt(Vector3f lookat)
	{
		front = (lookat - Position()).Normalized();
		auto worldUp = Vector3f { 0, 1, 0 };
		right = front.Cross(worldUp).Normalized();
		up = right.Cross(front).Normalized();
		q = Euler(up, front).ToQuaternion();
		std::cout << "SetLookAt: " << q << std::endl;
	}
};

enum CameraType {
	EulerType,
	QuaternionType,
	VectorType
};

class SwitchableCamera {
public:
	std::shared_ptr<Camera> pCamera;
	CameraType Type = CameraType::EulerType;
	SwitchableCamera(Vector3f position = { 0, 0, 0 }, Vector3f up = { 0, 1, 0 }, Vector3f front = { 0, 0, -1 })
	 : pCamera(std::make_shared<EulerCamera>(position, Euler(up, front))) {};

	void Switch(CameraType target)
	{
		std::shared_ptr<Bindable> pTarget = nullptr;
        pCamera->ReleaseTarget(pTarget);

		if (target == CameraType::EulerType)
		{
			auto up = pCamera->Up(), front = pCamera->Front();
			std::cout << "ToEuler: " << up << "\t" << front << std::endl;
			pCamera = std::make_shared<EulerCamera>(EulerCamera{ pCamera->Position(), Euler(up, front) });
			Type = target;
		}
		else if (target == CameraType::VectorType)
		{
			std::cout << "ToVector: " << pCamera->Up() << "\t" << pCamera->Front() << std::endl;
			pCamera = std::make_shared<VectorCamera>(pCamera->Position(), pCamera->Up(), pCamera->Front());
			std::cout << "EulerCamera: " << pCamera->Up() << "\t" << pCamera->Front() << std::endl;
			Type = target;
		}
		else if (target == CameraType::QuaternionType)
		{
			auto up = pCamera->Up(), front = pCamera->Front();
			pCamera = std::make_shared<QuaternionCamera>(pCamera->Position(), Euler(up, front).ToQuaternion());
		}

		if (pTarget)
		{
			pCamera->SetTarget(*pTarget);
		}
		else
		{
			pCamera->ReleaseTarget(pTarget);
		}
	}

	Camera& Get() {
		return *pCamera;
	}
};

class TransitionCamera {
private:
	std::shared_ptr<SwitchableCamera> pFrom, pTo, pTransition;
	int t = 0;
	const int DURATION = 5;
	bool transiting = false;
	Quaternion directionFrom, directionTo;
	Vector3f posFrom, posTo;
public:
	TransitionCamera():
		pFrom(std::make_shared<SwitchableCamera>()),
		pTo(std::make_shared<SwitchableCamera>()),
		pTransition(std::make_shared<SwitchableCamera>()),
        directionFrom(Quaternion{ 1, 0, 0, 0 }),
        directionTo(Quaternion{ 1, 0, 0, 0 })
	{
		pTransition->Switch(CameraType::QuaternionType);
	}

	void StartTransition() {
		transiting = true;
		pTransition->Get().SetPosition(pFrom->Get().Position());
		pTransition->Get().SetLookAt(pFrom->Get().Center());

        auto fromUp = pFrom->Get().Up(), fromFront = pFrom->Get().Front();
        directionFrom = Euler(fromUp, fromFront).ToQuaternion();

        auto toUp = pTo->Get().Up(), toFront = pTo->Get().Front();
        directionTo = Euler(toUp, toFront).ToQuaternion();

		std::cout << "StartTransition: " << directionTo.ToVector() << std::endl;

        posFrom = pFrom->Get().Position();
        posTo = pTo->Get().Position();
	}

	void Switch(CameraType type)
	{
		pFrom->Switch(type);
	}

    void SetTarget(Bindable& target)
    {
        pTo->Get().SetTarget(target);
    }

	CameraType Type()
	{
        return pFrom->Type;
	}

	void Update()
	{
		/*if (t == DURATION)
		{
			transiting = false;
			t = 0;
			std::swap(pTo, pFrom);
		}
		else if (transiting) {
			t += 1;
			auto q = directionFrom.Slerp(directionTo, float(t) / DURATION);
			auto pos = posFrom + (posTo - posFrom) * (t / float(DURATION));
			pTransition->Get().SetPosition(pos);
			pTransition->Get().SetLookAt(pos + q.ToVector());
		}*/
		if (transiting) {
			transiting = false;
            t = 0;
            std::swap(pTo, pFrom);
		}
	}
	
	Camera& Get() {
		if (transiting)
		{
			return pTransition->Get();
		}
		else
		{
			return pFrom->Get();
		}
	}
};