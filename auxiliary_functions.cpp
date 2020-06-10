#include <iostream>
#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include "auxiliary_functions.h"
#include "window_parameters.h"

using namespace std;

extern unsigned int map[Y_MAX][X_MAX];

float mapValue(float val, float fromMin, float fromMax, float toMin, float toMax)
{
	return (val - fromMin) * (toMax - toMin) / (fromMax - fromMin) * 1.0 + toMin;
}

bool isInFreeSpace(int x, int y)
{
	if (map[y][x] == 1)
	{
		return false;
	}
	return true;
}

bool isPathFree(int stX, int stY, int glX, int glY)
{
	float tempX = stX, tempY = stY;
	int difY = abs(glY - stY);
	int difX = abs(glX - stX);
	float angle = atan(difY*1.0 / difX * 1.0);

	if (stX < glX && stY != glY)
	{
		if (stY > glY)
		{
			while (((int)tempX != glX) && ((int)tempY != glY))
			{
				tempX += SAMPLING_STEP;
				tempY = stY * 1.0 - (tan(angle)*1.0*abs(tempX - stX));
				//drawMiddlePoints(tempX, tempY);
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
		else
		{
			while (((int)tempX != glX) && ((int)tempY != glY))
			{
				tempX += SAMPLING_STEP;
				tempY = stY * 1.0 + (tan(angle)*1.0*abs(tempX - stX));
				//drawMiddlePoints(tempX, tempY);
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
	}
	else if (stX > glX && stY != glY)
	{
		if (stY > glY)
		{
			while (((int)tempX != glX) && ((int)tempY != glY))
			{
				tempX -= SAMPLING_STEP;
				tempY = stY * 1.0 - (tan(angle)*1.0*abs(tempX - stX));
				//drawMiddlePoints(tempX, tempY);
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
		else
		{
			while (((int)tempX != glX) && ((int)tempY != glY))
			{
				tempX -= SAMPLING_STEP;
				tempY = stY * 1.0 + (tan(angle)*1.0*abs(tempX - stX));
				//drawMiddlePoints(tempX, tempY);
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
	}
	else if (stX == glX && stY != glY)
	{
		if (stY > glY)
		{
			while ((int)tempY != glY)
			{
				tempY -= SAMPLING_STEP;
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
		else
		{
			while ((int)tempY != glY)
			{
				tempY += SAMPLING_STEP;
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
	}
	else if (stY == glY && stX != glX)
	{
		if (stX > glX)
		{
			while ((int)tempX != glX)
			{
				tempX -= SAMPLING_STEP;
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
		else
		{
			while ((int)tempX != glX)
			{
				tempX += SAMPLING_STEP;
				if (!isInFreeSpace(tempX, tempY))
				{
					return false;
				}
			}
		}
	}

	return true;
}