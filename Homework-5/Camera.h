#pragma once
#include "stdafx.h"

#include "Geometry.h"
#include "Matrix.h"
#include "Euler.h"
#include "Drawable.h"

#include <cmath>
#include <cassert>
#include "Quaternion.h"

const auto worldUp = Vector3f{ 0, 1, 0 };

class Camera
{
protected:
	Vector3f up, right, front;
	Vector3f position;
	Bindable* pTarget = nullptr;  // 改用原始指针
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
		pTarget = &target;  // 使用原始指针，不进行所有权管理
	}
	
	Bindable* ReleaseTarget() {
		auto prev = pTarget;
		pTarget = nullptr;
		return prev;
	}
};

class VectorCamera : public Camera
{
public:
	Vector3f& Up() { return pTarget ? pTarget->Up() : up; }
	Vector3f& Right() { return pTarget ? pTarget->Right() : right; }
	Vector3f& Front() { return pTarget ? pTarget->Front() : front; }
	
	VectorCamera(Vector3f position = { 0, 0, 0 }, Vector3f upv = { 0, 1, 0 }, Vector3f frontv = { 0, 0, -1 })
	{
		// 首先标准化输入向量
		frontv = frontv.Normalized();
		upv = upv.Normalized();
		
		if (fabs(upv.Dot(frontv)) > 1e-5) {
			std::cerr << "VectorCamera: input up and front vectors are not orthogonal! Dot Value: " << upv.Dot(frontv) << std::endl;
			// 强制使用worldUp重建正交基
			upv = Vector3f{ 0, 1, 0 };
		}
		
		Camera::position = position;
		front = frontv;
		right = front.Cross(upv).Normalized();
		up = right.Cross(front).Normalized();
		
		// 验证正交性
		ValidateOrthogonality();
	}

	void Move(Vector3f v)
	{
        if (pTarget) {
            pTarget->Move(v);
            return;
        }
		Position() += v;
	}

	void Yaw(float rad)
	{
		// 绕up轴旋转front和right
		front = (Front() * cos(rad) - Right() * sin(rad)).Normalized();
		right = front.Cross(up).Normalized();
		up = right.Cross(front).Normalized();
		ValidateOrthogonality();
		if (pTarget) pTarget->Yaw(rad);
	}

	void Pitch(float rad)
	{
		// 绕right轴旋转front和up
		front = (Front() * cos(rad) + Up() * sin(rad)).Normalized();
		up = right.Cross(front).Normalized();
		right = front.Cross(up).Normalized();
		ValidateOrthogonality();
        if (pTarget) pTarget->Pitch(rad);
	}

	void Roll(float rad)
	{
		// 绕front轴旋转up和right
		Matrix3f rot(cos(rad), -sin(rad), 0.f,
					sin(rad), cos(rad), 0.f,
					0.f, 0.f, 1.f);
		right = (rot * Matrix3f::VMatrix(Right())).ToVector().Normalized();
		up = right.Cross(front).Normalized();
		right = front.Cross(up).Normalized(); // 再次确保right完全正交
		ValidateOrthogonality();
        if (pTarget) pTarget->Roll(rad);
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
		ValidateOrthogonality();
	}

private:
	void ValidateOrthogonality()
	{
		float frontUpDot = fabs(front.Dot(up));
		float rightUpDot = fabs(right.Dot(up));
		float frontRightDot = fabs(front.Dot(right));
		
		if (frontUpDot > 1e-5 || rightUpDot > 1e-5 || frontRightDot > 1e-5) {
			std::cerr << "VectorCamera: Vectors not orthogonal!" << std::endl;
			std::cerr << "front·up: " << frontUpDot 
				<< " right·up: " << rightUpDot 
				<< " front·right: " << frontRightDot << std::endl;
		}
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
		if (pTarget) {
			pTarget->Move(v);
			return;
		}
		Position() += v;
	};
	void Yaw(float rad)
	{
		euler.yaw += rad;
        if (pTarget) pTarget->Yaw(rad);
	}
	void Pitch(float rad)
	{
		euler.pitch += rad;
        if (pTarget) pTarget->Pitch(rad);
	}
	void Roll(float rad)
	{
		euler.roll += rad;
        if (pTarget) pTarget->Roll(rad);
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
	Vector3f& Up() { 
		if (pTarget) return pTarget->Up();
		// 通过四元数旋转世界up向量得到相机的up向量
		up = (q * Quaternion(0, 0, 1, 0) * q.Inversed()).ParseVector().Normalized();
		return up;
	}
	Vector3f& Front() { 
		if (pTarget) return pTarget->Front();
		// 初始front向量是(0,0,-1)，所以用Quaternion(0, 0, 0, -1)
		front = (q * Quaternion(0, 0, 0, -1) * q.Inversed()).ParseVector().Normalized();
		return front;
	}
	Vector3f& Right() { 
		if (pTarget) return pTarget->Right();
		right = Front().Cross(Up());  // 注意顺序，确保right指向正确的方向
		return right;
	}
	
	QuaternionCamera(Vector3f position = { 0, 0, 0 }, Quaternion q = { 1, 0, 0, 0 }) : q(q)
	{
		Position() = position;
		// 首先更新front和up向量
		front = q.ToVector();
		up = (q * Quaternion(0, 0, 1, 0) * q.Inversed()).ToVector().Normalized();
		right = front.Cross(up);  // 最后计算right向量
	}

	void Move(Vector3f v)
	{
		if (pTarget) {
			pTarget->Move(v);
			return;
		}
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
		Bindable* pTarget = pCamera->ReleaseTarget();

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
			std::cout << "VectorCamera: " << pCamera->Up() << "\t" << pCamera->Front() << std::endl;
			Type = target;
		}
		else if (target == CameraType::QuaternionType)
		{
			auto up = pCamera->Up(), front = pCamera->Front();
			pCamera = std::make_shared<QuaternionCamera>(pCamera->Position(), Euler(up, front).ToQuaternion());
			Type = target;
		}

		if (pTarget)
		{
			pCamera->SetTarget(*pTarget);
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
	const int DURATION = 60;
	bool transiting = false;
	Quaternion directionFrom, directionTo;
	Vector3f posFrom, posTo;
	Bindable* pTarget = nullptr;

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

	void Switch(CameraType type) {
		pFrom->Switch(type);
	}

	void SetTarget(Bindable* target) {
        pTarget = target;
        pTo->Get().SetTarget(*pTarget);
	}

	CameraType Type() {
		return pFrom->Type;
	}

	void Update() {
		if (transiting) {
			t += 1;
			if (t >= DURATION) {
				transiting = false;
				t = 0;
				std::swap(pTo, pFrom);
			}
			else {
				auto q = directionFrom.Slerp(directionTo, float(t) / DURATION);
				auto pos = posFrom + (posTo - posFrom) * (t / float(DURATION));
				pTransition->Get().SetPosition(pos);
				pTransition->Get().SetLookAt(pos + q.ToVector());
			}
		}
	}
	
	Camera& Get() {
		if (transiting) {
			return pTransition->Get();
		} else {
			return pFrom->Get();
		}
	}
};