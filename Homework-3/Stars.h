#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"

#include <random>
#include <iostream>

class Stars : public Drawable
{
	std::vector<std::pair<std::shared_ptr<Point>, double>> points;
	std::vector<pDrawable> objs;

	Matrix4f rotation, delta;
	float angle = 0;
public:
	Stars(size_t size) : points(), objs(), rotation(Matrix4f::Identity()),
	  delta(RotationMatrix({ 0, 1, 0 }, 0.5 / 30.0))
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(-0.5f, 0.5f);
		for (int i = 0; i < size; i++)
		{
			Vector3f pos = { dis(gen), dis(gen), dis(gen) };
			pos = pos.Normalized() * 100;
			
			auto point = std::make_shared<Point>(Vertex3f(pos, { 1, 1, 1 }), 2.0f);
			points.emplace_back(
				point,
				dis(gen) * 2 * PI
			);

			objs.push_back(point);
		};
	}

	void OnUpdate(int val) override
	{
		for (auto& p : points)
		{
			auto point = std::dynamic_pointer_cast<Point>(p.first);
			float color = sin(p.second);
			p.first->vertex.SetColor({ color, color, color });

			p.second += 1.0f / 30.0f * val * 0.25;
		}

		for (auto& o : objs)
		{
			o->Update(val);
		}

        rotation = rotation * delta;
	}

	void OnDraw() override
	{
		glMultMatrixf(rotation.getGlMatrix().data());
		for (auto& obj : objs)
		{
			obj->Draw();
		}
	};
};