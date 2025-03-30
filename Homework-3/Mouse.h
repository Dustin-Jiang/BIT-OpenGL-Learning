#pragma once
#include "stdafx.h"

class Mouse
{
	int prevX = -1;
	int prevY = -1;

public:
	int deltaX = 0;
	int deltaY = 0;

    int x = 0;
    int y = 0;

    bool IsLeftDown = false;
    bool IsRightDown = false;
	bool IsMiddleDown = false;

	static Mouse* GetInstance()
	{
		static Mouse instance;
		return &instance;
	}

	void Update(int x, int y)
	{
        this->x = x;
        this->y = y;
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

	void Update(int button, int state, int x, int y)
	{
		this->x = x;
		this->y = y;

        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            IsLeftDown = state == GLUT_DOWN;
            break;
        case GLUT_RIGHT_BUTTON:
            IsRightDown = state == GLUT_DOWN;
            break;
        case GLUT_MIDDLE_BUTTON:
            IsMiddleDown = state == GLUT_DOWN;
            break;
        }
	}

	bool IsClick()
	{
        if (IsLeftDown)
        {
            IsLeftDown = false;
            return true;
        }
        return false;
	}

private:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
};