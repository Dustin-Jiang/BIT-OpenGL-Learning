#pragma once
#include "stdafx.h"

#include "Vector.h"
#include "Drawable.h"
#include "Geometry.h"

class Planet : public Drawable
{
public:
	Vector3f pos;
	float radius;
	bool isWire = false;
	Sphere sphere, selection;
	Matrix4f spin, revolution, spinSum, revolutionSum;
	Track track;

	bool isSelected = false;

	Planet(Vector3f pos, Vector3f color, float radius, float spin = 0.0f, float revolution = 0.0f) : pos(pos), radius(radius),
		sphere(Sphere(Vertex3f{ {0, 0, 0}, color }, radius)),
		selection(Sphere(Vertex3f{ {0, 0, 0}, { 0, 1, 0} }, radius + 1)),
		revolution(Matrix4f::Identity()), spin(Matrix4f::Identity()), 
		spinSum(Matrix4f::Identity()), revolutionSum(Matrix4f::Identity()), track(1000, 1.0f)
	{
		selection.isWire = true;
		if (spin != 0.0f)
		{
			this->spin = RotationMatrix({ 0, 1, 0 }, spin / 30.0f);
		}
		if (revolution != 0.0f)
		{
			this->revolution = RotationMatrix({ 0, 1, 0 }, revolution / 30.0f);
		}
	};

	void BindTexture(std::shared_ptr<Texture> pTexture) {
		sphere.BindTexture(pTexture);
	}

	std::string GetName() { return name; }

private:
	std::wstring textureFileName;

protected:
	std::string name = "Planet";
	void OnDraw() override
	{
		track.Draw();

		glMultMatrixf(revolutionSum.getGlMatrix().data());
		glPushMatrix();

		glTranslatef(pos.x(), pos.y(), pos.z());
		if (isSelected) selection.Draw();

		glMultMatrixf(spinSum.getGlMatrix().data());

		sphere.Draw();

		glPopMatrix();
	}

	void OnUpdate(int interval) override
	{
		sphere.isWire = isWire;

		spinSum = spinSum * spin;
		revolutionSum = revolutionSum * revolution;

		track.AddPoint(Vertex3f{ WorldPosition(), {0.8, 0.8, 0.8} });
	}

public:
	Vector3f WorldPosition()
	{
		return RotatedPosition(pos, revolutionSum);
	}

	float GetDepth(Vector3f from, Vector3f dir)
	{

	}

	float CheckHit(Vector3f from, Vector3f dir, Vector3f camDir)
	{
		bool hit = WorldPosition().Distance(from, dir) < radius;
		return hit ? WorldPosition().Depth(from, camDir) : -1;
	}
};

class Sun : public Planet
{
public:
	Sun(Vector3f pos) : Planet(pos, { 1, 0.8, 0 }, 200.0f, 0.0f, 0.5f) {
		this->BindTexture(std::make_shared<Texture>(L"sun.bmp"));
		this->name = "Sun";
	};

	void OnDraw() override
	{
		GLfloat emission[] = { 1.0f, 0.8f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	
		Planet::OnDraw();
	
		GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
	}
};

class Mercury : public Planet
{
public:
	Mercury(Vector3f pos) : Planet(pos, { 0.5, 0.5, 0.5 }, 10.0f, 1.0f, 0.5f) {
		this->BindTexture(std::make_shared<Texture>(L"mercury.bmp"));
		this->name = "Mercury";
	};
};

class Venus : public Planet
{
public:
	Venus(Vector3f pos) : Planet(pos, { 0.8, 0.8, 0.8 }, 10.0f, 0.5f, 0.3f) {
		this->BindTexture(std::make_shared<Texture>(L"venus.bmp"));
		this->name = "Venus";
	};
};

class Moon : public Planet
{
public:
	Moon(Vector3f pos) : Planet ({ pos, {150.0 / 256,146.0 / 256,143.0 / 256}, 5.0f, 0.1f, 10.0f }) {
		this->BindTexture(std::make_shared<Texture>(L"moon.bmp"));
		this->name = "Moon";
	};
	void OnUpdate(int interval) override
	{
		Planet::OnUpdate(interval);
	}
};

class Earth : public Planet
{
	Moon moon;
public:
	Earth(Vector3f pos) : Planet(pos, { 0, 0.3, 1 }, 20.0f, 1.0f, 0.2f), moon({30.0,0.0,0.0}) {
		this->BindTexture(std::make_shared<Texture>(L"earth.bmp"));
		this->name = "Earth";
	};

	void OnDraw() override
	{
		track.Draw();

		glMultMatrixf(revolutionSum.getGlMatrix().data());
		glPushMatrix();

		glTranslatef(pos.x(), pos.y(), pos.z());

		if (isSelected) selection.Draw();

		moon.Draw();

		glMultMatrixf(spinSum.getGlMatrix().data());

		sphere.Draw();

		glPopMatrix();
	}
	void OnUpdate(int interval)
	{
		Planet::OnUpdate(interval);
		moon.isWire = isWire;
		moon.Update(interval);
	}
};

class Mars : public Planet
{
public:
	Mars(Vector3f pos) : Planet(pos, { 1, 0.4, 0 }, 15.0f, 0.5f, 0.1f) {
		this->BindTexture(std::make_shared<Texture>(L"mars.bmp"));
		this->name = "Mars";
	};
};

class Jupiter : public Planet
{
public:
	Jupiter(Vector3f pos) : Planet(pos, { 181.f / 256, 162.f / 256, 119.f / 256 }, 80.0f, 0.5f, 0.05f) {
		this->BindTexture(std::make_shared<Texture>(L"jupiter.bmp"));
		this->name = "Jupiter";
	};
};

class Saturn : public Planet
{
	Ring ring;
public:
	Saturn(Vector3f pos) : Planet(pos, { 213.f / 256, 181.f / 256, 122.f / 256 }, 70.0f, 0.5f, 0.03f),
		ring({ { 0,0,0 }, { 178.0f/256, 166.0f/256, 141.0f/256 } }, { 0,0.8,0.6 }, 80, 90, 250) {
		this->BindTexture(std::make_shared<Texture>(L"saturn.bmp"));
		this->name = "Saturn";
	};

	void OnDraw() override
	{
		track.Draw();

		glMultMatrixf(revolutionSum.getGlMatrix().data());
		glPushMatrix();

		glTranslatef(pos.x(), pos.y(), pos.z());

		if (isSelected) selection.Draw();

		glMultMatrixf(spinSum.getGlMatrix().data());

		sphere.Draw();
		ring.Draw();

		glPopMatrix();
	}
	void OnUpdate(int interval)
	{
		Planet::OnUpdate(interval);
		ring.isWire = isWire;
		ring.Update(interval);
	}
};

class Uranus : public Planet
{
public:
	Uranus(Vector3f pos) : Planet(pos, { 0.5, 0.5, 1 }, 30.0f, 0.5f, 0.02f) {
		this->BindTexture(std::make_shared<Texture>(L"uranus.bmp"));
		this->name = "Uranus";
	};
};

class Neptune : public Planet
{
public:
	Neptune(Vector3f pos) : Planet(pos, { 0, 0, 1 }, 30.0f, 0.5f, 0.01f) {
		this->BindTexture(std::make_shared<Texture>(L"nepture.bmp"));
		this->name = "Neptune";
	};
};