#pragma once
#include "stdafx.h"

class Mouse
{
	int prevX = -1;
	int prevY = -1;

public:
	int deltaX = 0;
	int deltaY = 0;

	static Mouse* GetInstance()
	{
		static Mouse instance;
		return &instance;
	}

	void Update(int x, int y)
	{
		if (prevX == -1)
		{
			prevX = x;
			prevY = y;
		}
		deltaX = x - prevX;
		deltaY = y - prevY;
		prevX = x;
		prevY = y;
	}

private:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
};