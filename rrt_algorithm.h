#ifndef RRT_ALGORITHM_INCLUDED_H
#define RRT_ALGORITHM_INCLUDED_H

#define MIN_DISTANCE_TO_NODE	2.0
#define R						1.0

void initRRT();
int* calculateNode(int x1, int y1, int x2, int y2);
int returnClosestNode(int rX, int rY);
bool solveRRT();
void drawParentMap();

#endif // !RRT_ALGORITHM_INCLUDED_H
