#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"

#include <functional>
#include <random>
#include <iostream>

class MovingSphere : public Drawable
{
	Sphere sphere;
	Arrow arrow;
	Vector3f velocity;
public:
	MovingSphere(Vertex3f vertex, float radius, unsigned int slices, unsigned int stacks) :
		sphere(vertex, radius, slices, stacks), velocity(),
		arrow({ vertex.pos, {0,1,0} }, { {0,1,0}, {0,1,0} })
	{
		ResetVelocity();
	};

	void OnUpdate(int val) override
	{
		auto delta = velocity * (1.0 * val / 1000);
		sphere.vertex.pos += delta;
		arrow.SetPosition(sphere.vertex.pos);
		arrow.SetDirection(velocity);

		arrow.Update(val);

		sphere.Update(val);
	};

	void OnDraw() override
	{
		arrow.Draw();
		sphere.Draw();
	};

	void SetVelocity(Vector3f v)
	{
		velocity = v;
		arrow.SetDirection(v);
	}

	void CalcVelocity(std::function<Vector3f(Vector3f)> op)
	{
		SetVelocity(op(velocity));
	}

	void ResetVelocity()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(-0.5f, 0.5f);

		SetVelocity(Vector3f{ dis(gen), dis(gen), dis(gen) } *0.5);
	}

	void ResetPosition()
	{
		sphere.vertex.pos = { 0, 0, 0 };
	};
};