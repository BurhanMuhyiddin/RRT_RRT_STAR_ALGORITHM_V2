#ifndef AUXILIARY_FUNCTIONS_INCLUDED_H
#define AUXILIARY_FUNCTIONS_INCLUDED_H

#define SAMPLING_STEP	0.1
#define INF				9999999

float mapValue(float val, float fromMin, float fromMax, float toMin, float toMax);
bool isPathFree(int stX, int stY, int glX, int glY);
bool isInFreeSpace(int x, int y);
int d2IndexToD1Index(int indexX, int indexY);
double calculateDistance(int x1, int y1, int x2, int y2);
int* generateRandomPoint();
bool isGoalReached(int x, int y);
int* d1IndexToD2Index(int index); 

#endif // !AUXILIARY_FUNCTIONS_INCLUDED_H
