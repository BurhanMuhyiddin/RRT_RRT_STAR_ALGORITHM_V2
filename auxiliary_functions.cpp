#include <iostream>
#include <vector>
#include <math.h>
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

int d2IndexToD1Index(int indexX, int indexY)
{
	return indexY + Y_MAX * indexX;
}

int* d1IndexToD2Index(int index)
{
	int temp[2] = { 0 };

	temp[1] = index % Y_MAX;
	temp[0] = (index - temp[1]) / Y_MAX;

	return temp;
}

double calculateDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

int* generateRandomPoint()
{
	int temp[2] = { 0, 0 };

	temp[0] = rand() % (X_MAX - 1);
	temp[1] = rand() % (Y_MAX - 1);

	return temp;
}

bool isInFreeSpace(int x, int y)
{
	if (map[y][x] == 1)
	{
		return false;
	}
	return true;
}

bool isGoalReached(int x, int y)
{
	if (map[y][x] == 6 || map[y][x] == 7)
	{
		return true;
	}
	return false;
}

double getNextPoint(int stX, int stY, int glX, int glY, double tX)  // c1 is the x coordinate of next point
{
	double temp = 0.0;

	int b = abs(stY - glY);
	int c = abs(stX - glX);
	double c1 = abs(tX - stX*1.0);
	double d = b * 1.0 * c1 / c / 1.0; // this is b1

	if (stX <= glX && stY >= glY)
	{
		temp = (stY*1.0 - d);
	}
	else if (stX >= glX && stY >= glY)
	{
		temp = (stY*1.0 - d);
	}
	else if (stX >= glX && stY <= glY)
	{
		temp = (stY*1.0 + d);
	}
	else if (stX <= glX && stY <= glY)
	{
		temp = (stY*1.0 + d);
	}

	return temp;
}

bool isPathFree(int stX, int stY, int glX, int glY)
{
	double tempX = stX, tempY = stY;

	while (1)
	{
		if (((int)tempX == glX) && ((int)tempY == glY))
		{
			break;
		}
		else if (tempX < glX && tempY != glY)
		{
			tempX += SAMPLING_STEP;
			tempY = getNextPoint(stX, stY, glX, glY, tempX);

			if (!isInFreeSpace(floor(tempX), floor(tempY)))
			{
				return false;
			}

		}
		else if (tempX > glX && tempY != glY)
		{
			tempX -= SAMPLING_STEP;
			tempY = getNextPoint(stX, stY, glX, glY, tempX);

			if (!isInFreeSpace(floor(tempX), floor(tempY)))
			{
				return false;
			}

		}
		else if (tempX == glX && tempY != glY)
		{
			if (tempY > glY)
			{
				tempY -= SAMPLING_STEP;
				if (!isInFreeSpace(floor(tempX), floor(tempY)))
				{
					return false;
				}
			}
			else
			{
				tempY += SAMPLING_STEP;
				if (!isInFreeSpace(floor(tempX), floor(tempY)))
				{
					return false;
				}
			}
		}
		else if (tempY == glY && tempX != glX)
		{
			if (tempX > glX)
			{
				tempX -= SAMPLING_STEP;
				if (!isInFreeSpace(floor(tempX), floor(tempY)))
				{
					return false;
				}
			}
			else
			{
				tempX += SAMPLING_STEP;
				if (!isInFreeSpace(floor(tempX), floor(tempY)))
				{
					return false;
				}
			}
		}
	}

	return true;
}