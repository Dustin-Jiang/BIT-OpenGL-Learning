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

void Triangle::OnDraw()
{
    glPushMatrix();
    auto pos = bottomCenter.pos;

    glColor3f(bottomCenter.color.x(), bottomCenter.color.y(), bottomCenter.color.z());
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(pos.x() - width / 2, pos.y(), pos.z());
    glVertex3f(pos.x() + width / 2, pos.y(), pos.z());
    glVertex3f(pos.x(), pos.y(), pos.z() - height);
    glEnd();

    glPopMatrix();
}

Circle::Circle(Vertex3f vertex, float radius, unsigned int slices) : vertex(vertex), radius(radius), slices(slices), vertices()
{
    // 遍历圆周
    for (int i = 0; i < slices; i++) {
        float theta = 2.0f * PI * i / slices; // θ 范围 [0, 2π]
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        // 将顶点加入数组：依次存放 x, y, z
        vertices.emplace_back(x, vertex.y(), z);
    }
};

void Circle::OnDraw()
{
    glPushMatrix();
    glTranslatef(vertex.x(), vertex.y(), vertex.z());
    glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
    glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
    glLineWidth(1.0f);
    // 绘制圆
    glBegin(GL_TRIANGLE_FAN);
    for (auto& v : vertices)
    {
        glVertex3f(v.x(), v.y(), v.z());
    }
    glEnd();
    glPopMatrix();
};

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

Cube::Cube(Vertex3f vertex, float size, Vector3f up, Vector3f front) : vertex(vertex), height(size), width(size),
    length(size), up(up), front(front), right(up.Cross(front)), vertices()
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

Cube::Cube(Vertex3f vertex, float length, float width, float height, Vector3f up, Vector3f front) : vertex(vertex), height(height), width(width),
    length(length), up(up), front(front), right(up.Cross(front)), vertices()
{
    float halfHeight = height / 2;
    float halfWidth = width / 2;
    float halfLength = length / 2;

    vertices = {
        { { halfWidth, halfHeight, halfLength }, vertex.color },
        { { halfWidth, halfHeight, -halfLength }, vertex.color },
        { { -halfWidth, halfHeight, -halfLength }, vertex.color },
        { { -halfWidth, halfHeight, halfLength }, vertex.color },
        { { halfWidth, -halfHeight, halfLength }, vertex.color },
        { { halfWidth, -halfHeight, -halfLength }, vertex.color },
        { { -halfWidth, -halfHeight, -halfLength }, vertex.color },
        { { -halfWidth, -halfHeight, halfLength }, vertex.color }
    };
}

void Cube::OnUpdate(int interval)
{
    front = front.Normalized();
    right = front.Cross(up).Normalized();
    up = right.Cross(front).Normalized();
    
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

    glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
    glLineWidth(1.0f);

    // 设置多边形绘制模式
    if (isWire) {
        // 如果是线框模式，直接设置所有面为线框
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        // 实体模式时，仍然保持背面为线框
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_LINE);
        
        // 启用面剔除，但先禁用它来绘制线框
        // 先绘制所有面的线框
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        DrawCube();
        
        // 再启用面剔除绘制实体面
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);  // 剔除背面
        glPolygonMode(GL_FRONT, GL_FILL);
        DrawCube();
        glDisable(GL_CULL_FACE);
    }

    glPopMatrix();
}

// 添加辅助方法来绘制立方体的几何体
void Cube::DrawCube()
{
    glBegin(GL_TRIANGLES);
    // 前面 - 使用逆时针顺序定义顶点（从外部看）
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[4].x(), vertices[4].y(), vertices[4].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    glVertex3f(vertices[3].x(), vertices[3].y(), vertices[3].z());
    
    // 右面
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
    glVertex3f(vertices[5].x(), vertices[5].y(), vertices[5].z());
    
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[5].x(), vertices[5].y(), vertices[5].z());
    glVertex3f(vertices[4].x(), vertices[4].y(), vertices[4].z());
    
    // 后面
    glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    
    glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    glVertex3f(vertices[5].x(), vertices[5].y(), vertices[5].z());
    
    // 左面
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    glVertex3f(vertices[3].x(), vertices[3].y(), vertices[3].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    
    // 上面
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[3].x(), vertices[3].y(), vertices[3].z());
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    
    glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
    glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
    glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
    
    // 下面
    glVertex3f(vertices[4].x(), vertices[4].y(), vertices[4].z());
    glVertex3f(vertices[5].x(), vertices[5].y(), vertices[5].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    
    glVertex3f(vertices[4].x(), vertices[4].y(), vertices[4].z());
    glVertex3f(vertices[6].x(), vertices[6].y(), vertices[6].z());
    glVertex3f(vertices[7].x(), vertices[7].y(), vertices[7].z());
    glEnd();
}

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

Vector3f RotatedPosition(Vector3f& pos, Matrix4f& m)
{
  VVector4f v{ pos.x(), pos.y(), pos.z(), 1 };
  auto result4f = m * v;
  Vector3f result3f = { result4f(0, 0), result4f(1, 0), result4f(2, 0) };
  return result3f;
}