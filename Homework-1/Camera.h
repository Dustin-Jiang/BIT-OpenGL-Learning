#pragma once
#include "stdafx.h"

#include "Geometry.h"

class Camera
{
public:
	Vector3f Up, Right, Front;
	Vector3f Position;

	Camera(Vector3f position = { 0, 0, 0 }, Vector3f up = { 0, 1, 0 }, Vector3f front = { 0, 0, -1 }) : Position(position), Up(up), Front(front)
	{
		Right = Up.Cross(Front);
	};

	void Move(Vector3f v)
	{
		Position += v;
	};

	void Yaw(float rad)
	{
		Front = (Front * cos(rad) + Right * sin(rad)).Normalized();
		Right = Up.Cross(Front).Normalized();
	}

	void Pitch(float rad)
	{
		Front = (Front * cos(rad) + Up * sin(rad)).Normalized();
		Up = Right.Cross(Front).Normalized();
	}
};