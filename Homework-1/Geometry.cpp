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
		glColor3f(v.color.x(), v.color.y(), v.color.z());
		glVertex3f(v.x(), v.y(), v.z());
	}
	glEnd();
}

void Point::OnDraw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
	glVertex3f(vertex.x(), vertex.y(), vertex.z());
	glEnd();
}

Sphere::Sphere(Vertex3f vertex, float radius, unsigned int slices, unsigned int stacks) : vertex(vertex),
radius(radius), slices(slices), stacks(stacks), vertices() 
{
	// 遍历纬度
	for (int i = 0; i < stacks; i++) {
		float phi = PI * i / (stacks - 1); // φ 范围 [0, π]
		float y = radius * cos(phi);
		float r = radius * sin(phi);

		// 存储当前纬度的所有顶点
		std::vector<Vector3f> row;
		for (int j = 0; j < slices; j++) {
			float theta = 2.0f * PI * j / slices; // θ 范围 [0, 2π]
			float x = r * cos(theta);
			float z = r * sin(theta);
			// 将顶点加入数组：依次存放 x, y, z
			row.emplace_back(x, y, z);
		}
		vertices.push_back(row);
	}
};

void Sphere::OnDraw()
{
	glPushMatrix();
	glTranslatef(vertex.x(), vertex.y(), vertex.z());
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
	glLineWidth(1.0f);

	// 绘制球体
	for (int i = 0; i < stacks - 1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= slices; j++) {
			// 当前纬度点
			Vector3f v1 = vertices[i][j % slices];
			// 下一纬度点
			Vector3f v2 = vertices[i + 1][j % slices];

			glVertex3f(v1.x(), v1.y(), v1.z());
			glVertex3f(v2.x(), v2.y(), v2.z());
		}
		glEnd();
	}

	glPopMatrix();
}