#include <iostream>
#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <stdlib.h>
#include <vector>
#include "rrt_algorithm.h"
#include "window_parameters.h"
#include "auxiliary_functions.h"

using namespace std;

vector < vector<int> > nodes;
int parent[Y_MAX][X_MAX] = { 0 };

extern float startX, startY;

void initRRT()
{
	for (int i = 0; i < X_MAX; i++)
	{
		for (int j = 0; j < Y_MAX; j++)
			parent[j][i] = INF;
	}
}

void drawParentMap()
{
	for (int i = 0; i < X_MAX; i++)
	{
		for (int j = 0; j < Y_MAX; j++)
			cout << parent[j][i] << " ";
		cout << endl;
	}

	cout << endl << "----------------------------------------------------------------------" << endl;
}

int returnClosestNode(int rX, int rY)
{
	unsigned int minDistance = 99999;
	int minIndex = 0;

	for (int i = 0; i < nodes.size(); i++)
	{
		int x = nodes[i][0];
		int y = nodes[i][1];

		double distance = calculateDistance(x, y, rX, rY);

		if (distance <= minDistance)
		{
			minDistance = distance;
			minIndex = i;
		}
	}

	return minIndex;
}

int* calculateNode(int stX, int stY, int glX, int glY)
{
	int temp[2];

	double a = MIN_DISTANCE_TO_NODE;
	double b = calculateDistance(stX, stY, glX, glY);
	double c = abs(stY*1.0 - glY*1.0);
	double d = a * c*1.0 / b / 1.0;
	double f = sqrt(pow(a, 2) - pow(d, 2));

	if (stX <= glX && stY >= glY)
	{
		temp[0] = (int)(stX*1.0 + f);
		temp[1] = (int)(stY*1.0 - d);
	}
	else if (stX >= glX && stY >= glY)
	{
		temp[0] = (int)(stX*1.0 - f);
		temp[1] = (int)(stY*1.0 - d);
	}
	else if (stX >= glX && stY <= glY)
	{
		temp[0] = (int)(stX*1.0 - f);
		temp[1] = (int)(stY*1.0 + d);
	}
	else if (stX <= glX && stY <= glY)
	{
		temp[0] = (int)(stX*1.0 + f);
		temp[1] = (int)(stY*1.0 + d);
	}

	return temp;
}

bool solveRRT()
{
	int* newNode;
	int nodeIndex = 0;

	int* temp = generateRandomPoint(); // Generated random point
	int x1 = temp[0], y1 = temp[1];
	int x2 = 0, y2 = 0;

	if (isInFreeSpace(x1, y1))
	{
		nodeIndex = returnClosestNode(x1, y1);
		x2 = nodes[nodeIndex][0];
		y2 = nodes[nodeIndex][1];
		if (calculateDistance(x2, y2, x1, y1) > MIN_DISTANCE_TO_NODE)
		{
			newNode = calculateNode(x2, y2, x1, y1);
			int newX = newNode[0];
			int newY = newNode[1];
			if (isGoalReached(newX, newY))
			{
				return true;
			}
			//if (isInFreeSpace(newX, newY))
			if (isPathFree(x2, y2, newX, newY))
			{
				vector <int> temp;
				temp.push_back(newX); temp.push_back(newY);
				nodes.push_back(temp);

				parent[newY][newX] = d2IndexToD1Index(x2, y2);
			}
		}
		//draw line between new node and temp
		//drawLine(x2, y2, newX, newY);
		//parent
	}

	return false;
}