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
    glTranslatef(pos.x(), pos.y(), pos.z());
    glMultMatrixf(Euler { up, front }.ToRotateMatrix().getGlMatrix().data());

    GLfloat amb[4] = { 0.4, 0.4, 0.4, 1 };
    GLfloat dif[4] = { bottomCenter.color.x(), bottomCenter.color.y(), bottomCenter.color.z(), 1 };
    GLfloat spe[4] = { 0.1, 0.1, 0.1, 1 };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);

    glColor3f(bottomCenter.color.x(), bottomCenter.color.y(), bottomCenter.color.z());
    glBegin(GL_TRIANGLES);
    Vector3f n = (up.Normalized() + front.Normalized()).Normalized();
    glNormal3f(n.x(), n.y(), n.z());
    glVertex3f(-width / 2, 0, 0);
    glVertex3f(width / 2, 0, 0);
    glVertex3f(0, 0, -height);
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

    GLfloat amb[4] = { 0.4, 0.4, 0.4, 1 };
    GLfloat dif[4] = { vertex.color.x(), vertex.color.y(), vertex.color.z(), 1 };
    GLfloat spe[4] = { 0.1, 0.1, 0.1, 1 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);

    glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
    glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
    glLineWidth(1.0f);
    // 绘制圆
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0); // 假设圆在XZ平面上，法向量朝上
    for (auto& v : vertices)
    {
        glVertex3f(v.x(), v.y(), v.z());
    }
    glEnd();
    glPopMatrix();
};

Sphere::Sphere(Vertex3f vertex, float radius) : vertex(vertex),
radius(radius), slices(radius > 1.f ? (int)(ceil(log10(radius)) * 20) : 20), stacks(radius > 1.f ? (int)(ceil(log10(radius)) * 20) : 20), vertices()
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
    glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());

    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glColor3f(1.0f, 1.0f, 1.0f); // Ensure base color is white
    } else {
        GLfloat amb[4] = { 0.4, 0.4, 0.4, 1 };
        GLfloat dif[4] = { vertex.color.x(), vertex.color.y(), vertex.color.z(), 1 };
        GLfloat spe[4] = { 0.1, 0.1, 0.1, 1 };
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        
        glDisable(GL_TEXTURE_2D);
        glColor3f(vertex.color.x(), vertex.color.y(), vertex.color.z());
    }
    glPolygonMode(GL_FRONT_AND_BACK, isWire ? GL_LINE : GL_FILL);
    glLineWidth(1.0f);

    // 绘制球体
    for (int i = 0; i < stacks - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; j++) {
            // 当前纬度点
            Vector3f v1 = vertices[i][j % slices];
            Vector3f n1 = v1.Normalized();
            // 下一纬度点
            Vector3f v2 = vertices[i + 1][j % slices];
            Vector3f n2 = v2.Normalized();

            Vector2f texCoord1 = { 1 - (float)j / (float)slices, 1 - (float)i / (float)(stacks - 1) };
            Vector2f texCoord2 = { 1 - (float)j / (float)slices, 1 - (float)(i + 1) / (float)(stacks - 1) };

            glTexCoord2f(texCoord1.x(), texCoord1.y());
            glNormal3f(n1.x(), n1.y(), n1.z());
            glVertex3f(v1.x(), v1.y(), v1.z());
            
            glTexCoord2f(texCoord2.x(), texCoord2.y());
            glNormal3f(n2.x(), n2.y(), n2.z());
            glVertex3f(v2.x(), v2.y(), v2.z());
        }
        glEnd();
    }

    // 如果启用了纹理，在绘制完成后禁用
    if (texture) {
        glDisable(GL_TEXTURE_2D);
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

    GLfloat amb[4] = { 0.4, 0.4, 0.4, 1 };
    GLfloat dif[4] = { vertex.color.x(), vertex.color.y(), vertex.color.z(), 1 };
    GLfloat spe[4] = { 0.1, 0.1, 0.1, 1 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);		//设置环境光材质属性，背面和正面使用相同的设置
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);		//设置散射光材质属性，背面和正面使用相同的设置
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);	//设置镜面反射光材质属性，背面和正面使用相同的设置
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
    glBegin(GL_QUADS);

    // Front face (+Z)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(&vertices[3].pos.x());
    glVertex3fv(&vertices[0].pos.x());
    glVertex3fv(&vertices[4].pos.x());
    glVertex3fv(&vertices[7].pos.x());

    // Back face (-Z)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3fv(&vertices[1].pos.x());
    glVertex3fv(&vertices[2].pos.x());
    glVertex3fv(&vertices[6].pos.x());
    glVertex3fv(&vertices[5].pos.x());

    // Top face (+Y)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(&vertices[2].pos.x());
    glVertex3fv(&vertices[1].pos.x());
    glVertex3fv(&vertices[0].pos.x());
    glVertex3fv(&vertices[3].pos.x());

    // Bottom face (-Y)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(&vertices[7].pos.x());
    glVertex3fv(&vertices[4].pos.x());
    glVertex3fv(&vertices[5].pos.x());
    glVertex3fv(&vertices[6].pos.x());
    
    // Right face (+X)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(&vertices[0].pos.x());
    glVertex3fv(&vertices[1].pos.x());
    glVertex3fv(&vertices[5].pos.x());
    glVertex3fv(&vertices[4].pos.x());

    // Left face (-X)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(&vertices[2].pos.x());
    glVertex3fv(&vertices[3].pos.x());
    glVertex3fv(&vertices[7].pos.x());
    glVertex3fv(&vertices[6].pos.x());

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

    Vector3f n = up.Normalized();
    glNormal3f(n.x(), n.y(), n.z());
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

Vector3f CalculateNormal(Vector3f& v1, Vector3f& v2, Vector3f& v3)
{
    Vector3f normal, dirv2_v1, dirv2_v3;
    for (int i = 0; i < 3; i++)
    {
        dirv2_v1[i] = v1[i] - v2[i];
        dirv2_v3[i] = v3[i] - v2[i];
    }
    //叉乘计算法线方向
    normal[0] = dirv2_v1[1] * dirv2_v3[2] - dirv2_v1[2] * dirv2_v3[1];
    normal[1] = dirv2_v1[2] * dirv2_v3[0] - dirv2_v1[0] * dirv2_v3[2];
    normal[2] = dirv2_v1[0] * dirv2_v3[1] - dirv2_v1[1] * dirv2_v3[0];
    return normal.Normalized();
}