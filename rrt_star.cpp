#include <iostream>
#include <vector>
#include "rrt_star.h"
#include "rrt_algorithm.h"
#include "window_parameters.h"
#include "auxiliary_functions.h"

using namespace std;

int costMap[Y_MAX][X_MAX];
int neighbourMap[Y_MAX][X_MAX] = { 0 };		// 1 indicates nodes
extern int theLastPointX;
extern int theLastPointY;

extern bool isDrawMap;
extern vector < vector<int> > nodes;
extern int parent[Y_MAX][X_MAX];
extern float startX, startY;

void initRRTStar()
{
	for (int i = 0; i < X_MAX; i++)
	{
		for (int j = 0; j < Y_MAX; j++)
			costMap[j][i] = INF;
	}

	costMap[(int)startY][(int)startX] = 0;
	neighbourMap[(int)startY][(int)startX] = 1;
}

bool solveRRTStar()
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
				theLastPointX = newX;
				theLastPointY = newY;
				parent[newY][newX] = d2IndexToD1Index(x2, y2);
				return true;
			}
			if (isPathFree(x2, y2, newX, newY))
			{
				unsigned int minDist = 99999999;
				unsigned int minX, minY = 0;
				unsigned int tmpCost = 0;
				vector <vector <int>> tmpNeighbours;
				for (int i = 0; i < RADIUS; i++)
				{
					for (int j = newX - i; j < newX + i; j++)
					{
						if (j > 0 && j < X_MAX && neighbourMap[newY - i][j] == 1)
						{
							vector <int> tmpHolder;
							tmpHolder.push_back(j);	tmpHolder.push_back(newY - i);	tmpNeighbours.push_back(tmpHolder);
							if ((newY - i != y2 && j != x2) && newY - i > 0)
							{
								tmpCost = costMap[newY-i][j] + calculateDistance(newX, newY, j, newY - i);
								if (tmpCost < minDist)
								{
									minDist = tmpCost;
									minX = j; minY = newY - i;
								}
							}
						}
					}
					for (int j = newX - i; j < newX + i; j++) 
					{
						if (j > 0 && j < X_MAX && neighbourMap[newY + i][j] == 1)
						{
							vector <int> tmpHolder;
							tmpHolder.push_back(j);	tmpHolder.push_back(newY + i);	tmpNeighbours.push_back(tmpHolder);
							if ((newY + i != y2 && j != x2) && newY + i < Y_MAX)
							{
								tmpCost = costMap[newY+i][j] + calculateDistance(newX, newY, j, newY + i);
								if (tmpCost < minDist)
								{
									minDist = tmpCost;
									minX = j; minY = newY + i;
								}
							}
						}
					}
					for (int j = newY - i - 1; j < newY + i - 1; j++)
					{
						if (j > 0 && j < Y_MAX && neighbourMap[j][newX - i] == 1)
						{
							vector <int> tmpHolder;
							tmpHolder.push_back(newX - i);	tmpHolder.push_back(j);	tmpNeighbours.push_back(tmpHolder);
							if ((j != y2 && newX - i != x2) && newX - i > 0)
							{
								tmpCost = costMap[j][newX - i] + calculateDistance(newX, newY, newX - i, j);
								if (tmpCost < minDist)
								{
									minDist = tmpCost;
									minX = newX - i; minY = j;
								}
							}
						}
					}
					for (int j = newY - i - 1; j < newY + i - 1; j++)
					{
						if (j > 0 && j < Y_MAX && neighbourMap[j][newX + i] == 1)
						{
							vector <int> tmpHolder;
							tmpHolder.push_back(newX + i);	tmpHolder.push_back(j);	tmpNeighbours.push_back(tmpHolder);
							if ((j != y2 && newX + i != x2) && newX + i < X_MAX)
							{
								tmpCost = costMap[j][newX + i] + calculateDistance(newX, newY, newX + i, j);
								if (tmpCost < minDist)
								{
									minDist = tmpCost;
									minX = newX + i; minY = j;
								}
							}
						}
					}
				}
			
				vector <int> temp;
				temp.push_back(newX); temp.push_back(newY);
				nodes.push_back(temp);

				neighbourMap[newY][newX] = 1;

				parent[newY][newX] = d2IndexToD1Index(minX, minY);
				costMap[newY][newX] = minDist;

				// Rewiring
				minDist = 99999999;
				minX, minY = 0;

				for (int i = 0; i < tmpNeighbours.size(); i++)
				{
					if (tmpNeighbours[i][0] != minX && tmpNeighbours[i][1] != minY)
					{
						unsigned int tX = tmpNeighbours[i][0];
						unsigned int tY = tmpNeighbours[i][1];
						tmpCost = costMap[tY][tX] + calculateDistance(newX, newY, tX, tY);
						if (tmpCost < minDist)
						{
							minDist = tmpCost;
							minX = tX; minY = tY;
						}
					}
				}

				parent[minY][minX] = d2IndexToD1Index(newX, newY);
				costMap[minY][minX] = minDist;
			}
		}
		//draw line between new node and temp
		//drawLine(x2, y2, newX, newY);
		//parent
	}

	return false;
}