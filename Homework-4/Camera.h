#pragma once
#include "stdafx.h"

#include "Geometry.h"
#include "Matrix.h"
#include "Euler.h"

#include <cmath>
#include <cassert>

const auto worldUp = Vector3f{ 0, 1, 0 };

class Camera
{
public:
	Vector3f up, right, front;
	Vector3f Position;
	Camera() = default;
	virtual void Move(Vector3f v) = 0;
	virtual void Yaw(float rad) = 0;
	virtual void Pitch(float rad) = 0;
	virtual void Roll(float rad) = 0;
	virtual Vector3f Center() = 0;
	virtual void SetPosition(Vector3f pos) = 0;
	virtual void SetLookAt(Vector3f lookat) = 0;
	virtual Vector3f& Up() { return up; }
	virtual Vector3f& Right() { return right; }
	virtual Vector3f& Front() { return front; }
};

class VectorCamera : public Camera
{
public:
	VectorCamera(Vector3f position = { 0, 0, 0 }, Vector3f up = { 0, 1, 0 }, Vector3f front = { 0, 0, -1 }) 
	{
		Position = position;
		Up() = up;
		Front() = front;
		Right() = Front().Cross(Up());
	}

	void Move(Vector3f v)
	{
		Position += v;
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
		return Position + Front();
	}

	void SetPosition(Vector3f pos)
	{
		Position = pos;
	}

	void SetLookAt(Vector3f lookat)
	{
		Front() = (lookat - Position).Normalized();
		Right() = Front().Cross(worldUp).Normalized();
		Up() = Right().Cross(Front()).Normalized();
	}
};

class EulerCamera : public Camera
{
	Euler euler;
public:
	Vector3f& Up() { up = euler.UpVector(); return up; }
	Vector3f& Front() { front = euler.FrontVector(); return front; }
	Vector3f& Right() { right = euler.FrontVector().Cross(up); return right; }
	
	EulerCamera(Vector3f position = { 0, 0, 0 }, Euler euler = { 0.0f, 0.0f, 0.0f }): euler(euler)
	{
		Position = position;
		up = euler.UpVector();
		front = euler.FrontVector();
		right = Front().Cross(Up());
	}
	void Move(Vector3f v)
	{
		Position += v;
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
		return Position + Front();
	}
	void SetPosition(Vector3f pos)
	{
		Position = pos;
	}
	void SetLookAt(Vector3f lookat)
	{
		auto front = (lookat - Position).Normalized();
		auto right = Front().Cross(worldUp).Normalized();
		auto up = Right().Cross(Front()).Normalized();
        euler = Euler(up, front);
	}
};