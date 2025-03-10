#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"

#include <random>
#include <iostream>

class MovingSphere : public Sphere
{
	Vector3f velocity;
public:
	MovingSphere(Vertex3f vertex, float radius, unsigned int slices, unsigned int stacks) : 
		Sphere(vertex, radius, slices, stacks), velocity()
	{
		ResetVelocity();
	};

	void OnUpdate(int val) override
	{
		vertex.x() += velocity.x() * val;
		vertex.y() += velocity.y() * val;
		vertex.z() += velocity.z() * val;
	}

	void SetVelocity(Vector3f v)
	{
		velocity = v;
	}

	void ResetVelocity()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(-0.5f, 0.5f);

		SetVelocity(Vector3f{ dis(gen), dis(gen), dis(gen) }.Normalized() * 0.25);
	}
};