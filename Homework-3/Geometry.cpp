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
    
    glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
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
};

Cube::Cube(Vertex3f vertex, float size, Vector3f up, Vector3f front) : vertex(vertex), size(size),
    up(up), front(front), right(up.Cross(front)), vertices()
{
    float half = size / 2;
    vertices = {
        { { half, half, half }, vertex.color },
        { { half, half, -half }, vertex.color },
        { { -half, half, -half }, vertex.color },
        { { -half, half, half }, vertex.color },
        { { half, -half, half }, vertex.color },
        { { half, -half, -half }, vertex.color },
        { { -half, -half, -half }, vertex.color },
        { { -half, -half, half }, vertex.color }
    };
};

void Cube::OnUpdate(int interval)
{
    front = front.Normalized();
    right = up.Cross(front).Normalized();
    up = front.Cross(right).Normalized();
    
    rotation = Matrix4f(right.x(), up.x(), front.x(), 0.0,
        right.y(), up.y(), front.y(), 0.0,
        right.z(), up.z(), front.z(), 0.0,
        0.0f, 0.0f, 0.0f, 1.0f);
}

void Cube::OnDraw()
{
    glPushMatrix();
    glTranslatef(vertex.x(), vertex.y(), vertex.z());
    glMultMatrixf(rotation.getGlMatrix().data());

    glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
    glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
    glLineWidth(1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    glVertex3f(vertices[3].x(), vertices[3].y(), vertices[3].z());
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(vertices[4].x(), vertices[4].y(), vertices[4].z());
    glVertex3f(vertices[5].x(), vertices[5].y(), vertices[5].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[4].x(), vertices[4].y(), vertices[4].z());
    glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
    glVertex3f(vertices[5].x(), vertices[5].y(), vertices[5].z());
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    glVertex3f(vertices[3].x(), vertices[3].y(), vertices[3].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    glEnd();

    glPopMatrix();
};

Cylinder::Cylinder(Vertex3f bottom, Vertex3f to, float radius, unsigned int slices) : bottom(bottom),
to(to), radius(radius), slices(slices), vertices(), top({0, to.pos.Length(), 0}, bottom.color)
{
    for (int i = 0; i < slices; i++)
    {
        float theta = 2 * PI / (slices - 1) * i;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        vertices.push_back({ { x, 0, z }, bottom.color });
    }
};

void Cylinder::OnDraw()
{
    glPushMatrix();
    glTranslatef(bottom.x(), bottom.y(), bottom.z());

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(bottom.color.x(), bottom.color.y(), bottom.color.z());
    glLineWidth(1.0f);

    glBegin(GL_TRIANGLE_STRIP);
    for (auto v : vertices)
    {
        glVertex3f(v.x(), v.y(), v.z());
        glVertex3f(0, 0, 0);
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    for (auto v : vertices)
    {
        glVertex3f(v.x(), v.y(), v.z());
        glVertex3f(top.x(), top.y(), top.z());
    }
    glEnd();

    glPopMatrix();
}

void Cylinder::OnUpdate(int val)
{
    top.pos = { 0, to.pos.Length(), 0 };
}

Arrow::Arrow(Vertex3f from, Vertex3f direction) : line({ from, { direction.pos, direction.color } }, LINE_TYPE::Lines, 2),
cylinder(from, direction, 0.5f, 20), from(from), direction(direction)
{
    SetDirection(direction.pos);
}

void Arrow::OnDraw()
{
    glPushMatrix();
    glTranslatef(from.x(), from.y(), from.z());
    line.Draw();

    glPushMatrix();
    glTranslatef(direction.x(), direction.y(), direction.z());
    glRotatef(rotationDegree, rotationAxis.x(), rotationAxis.y(), rotationAxis.z());
    cylinder.Draw();
    glPopMatrix();

    glPopMatrix();
}

void Arrow::OnUpdate(int val)
{
    line.vertices[1] = { direction.pos, direction.color };
    line.Update(val);

    cylinder.to = { {0.0f, direction.pos.Length(), 0.0f}, cylinder.to.color };
    cylinder.Update(val);
}

void Arrow::SetPosition(Vector3f pos) { from.pos = pos; }
void Arrow::SetDirection(Vector3f dir)
{
    if (!dir.Length()) return ;
    direction.pos = dir;
    if (dir.Normalized() == Vector3f {0, 1, 0}) return;
    Vector3f worldUp = { 0, 1, 0 };
    rotationAxis = worldUp.Cross(dir).Normalized();
    rotationDegree = acos(worldUp.Dot(dir.Normalized())) * 180 / PI;
}

Ring::Ring(Vertex3f vertex, Vector3f up, float innerRadius, float outerRadius, unsigned int slices) : pos(vertex.pos),
color(vertex.color), vertices(), up(up), innerRadius(innerRadius), outerRadius(outerRadius), slices(slices)
{
    float theta = 2 * PI / (slices - 1);
    for (int i = 0; i < slices; i++)
    {
        float x = cos(theta * i);
        float z = sin(theta * i);
        vertices[0].push_back({x * innerRadius, 0, z * innerRadius});
        vertices[1].push_back({x * outerRadius, 0, z * outerRadius});
    }
}

void Ring::OnDraw()
{
    glTranslatef(pos.x(), pos.y(), pos.z());
    glPushMatrix();

    glMultMatrixf(rotation.getGlMatrix().data());
    glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
    glColor3f(color.x(), color.y(), color.z());
    glLineWidth(1.0f);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < slices; i++)
    {
        auto v = vertices[0][i];
        glVertex3f(v.x(), v.y(), v.z());
        v = vertices[1][i];
        glVertex3f(v.x(), v.y(), v.z());
    }

    glEnd();
    glPopMatrix();
}

void Ring::OnUpdate(int val)
{
    if (up == Vector3f{ 0, 1, 0 }) return;
    auto axis = Vector3f{ 0, 1, 0 }.Cross(up).Normalized();
    auto angle = acos(Vector3f{ 0, 1, 0 }.Dot(up)) / acos(-1) * 180;
    rotation = RotationMatrix(up, angle);
}

Track::Track(size_t size, float width) : size(size), points({}), width(width)
{
    points.resize(size, Point { {{0,0,0},{0,0,0}}, width });
};

void Track::AddPoint(Vertex3f point)
{
    count = (count + 1) % sampling;
    if (count != 0)
    {
      return ;
    }
    points[current % size] = { point, width };
    current++;
    if (current == size * 2) current = size;
}

void Track::OnDraw()
{
    for (size_t i = 0; i < std::min(current, size); i++)
    {
        points[i].Draw();
    }
}

void Track::OnUpdate(int val)
{
    for (size_t i = 0; i < std::min(current, size); i++)
    {
        points[i].Update(val);
    }
}

Matrix4f RotationMatrix(Vector3f axis, float deg)
{
  float rad = deg * PI / 180;
  Vector3f v = axis.Normalized();
  VVector3f u{ v.x(), v.y(), v.z() };
  Matrix3f ux{ 0 , -v.z(), v.y(), v.z(), 0, -v.x(), -v.y(), v.x(), 0 };
  Matrix3f i = Matrix3f::Identity();
  auto result3f = i * cos(rad) + ux * sin(rad) + (u * u.Transpose()) * (1 - cos(rad));
  Matrix4f result{
    result3f(0,0), result3f(0,1), result3f(0,2), 0,
    result3f(1,0), result3f(1,1), result3f(1,2), 0,
    result3f(2,0), result3f(2,1), result3f(2,2), 0,
    0, 0, 0, 1 };
  return result;
}

Vector3f RotatedPosition(Vector3f& pos, Matrix4f& m)
{
  VVector4f v{ pos.x(), pos.y(), pos.z(), 1 };
  auto result4f = m * v;
  Vector3f result3f = { result4f(0, 0), result4f(1, 0), result4f(2, 0) };
  return result3f;
}