#pragma once
#include "stdafx.h"

#include "Geometry.h"

Line::Line(std::vector<Vertex3f> vertices, LINE_TYPE type = LINE_TYPE::Lines) : vertices(vertices), type(type), width(0.f) {}
Line::Line(std::vector<Vertex3f> vertices, LINE_TYPE type = LINE_TYPE::Lines, float width = 0.0f) : vertices(vertices), type(type), width(width) {}

void Line::SetWidth(float w)
{
	width = w;
}

void Line::OnDraw()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(width);
	glBegin(type);
	for (auto& v : vertices)
	{
		glColor3f(v.color.x, v.color.y, v.color.z);
		glVertex3f(v.x, v.y, v.z);
	}
	glEnd();
}