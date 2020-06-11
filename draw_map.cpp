#include <iostream>
#include <vector>
#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <string.h>
#include "draw_map.h"
#include "window_parameters.h"
#include "auxiliary_functions.h"

using namespace std;

extern bool isDrawMap;
extern vector < vector<int> > nodes;

unsigned int map[Y_MAX][X_MAX] = { 0 };
extern int parent[Y_MAX][X_MAX];

void visualizeNodes()
{
	glPointSize(3);
	glColor3f(0.5, 0.75, 0.12);

	for (int i = 0; i < nodes.size(); i++)
	{
		glBegin(GL_POINTS);

		glVertex2f(nodes[i][0], nodes[i][1]);

		glEnd();
	}
}

void visualizeConnectionsBetweenNodes()
{
	for (int i = 0; i < X_MAX; i++)
	{
		for (int j = 0; j < Y_MAX; j++)
		{
			if (parent[j][i] != INF)
			{
				int pX = d1IndexToD2Index(parent[j][i])[0]; int pY = d1IndexToD2Index(parent[j][i])[1];
				int cX = i; int cY = j;

				drawLine(pX, pY, cX, cY);
			}
		}
	}
}

void drawLine(int x1, int y1, int x2, int y2)
{
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINES);

	glVertex2i(x1, y1);
	glVertex2i(x2, y2);

	glEnd();
}

void loadMapNone()
{
	emptyMap();

	// TOP
	for (int i = 0; i < X_MAX; i++)
	{
		map[0][i] = 1;
	}
	// BOTTOM
	for (int i = 0; i < X_MAX; i++)
	{
		map[Y_MAX - 1][i] = 1;
	}
	// LEFT
	for (int i = 0; i < Y_MAX; i++)
	{
		map[i][0] = 1;
	}
	// RIGHT
	for (int i = 0; i < Y_MAX; i++)
	{
		map[i][X_MAX - 1] = 1;
	}

	isDrawMap = true;
}

void loadMapSparse()
{
	emptyMap();
	loadMapNone();

	int temp = (X_MAX - 2*BARRIER_THICKNESS - 3*MARGIN) / 2;

	// Left Top Square
	for (int i = BARRIER_THICKNESS+MARGIN; i < BARRIER_THICKNESS + MARGIN + temp; i++)
	{
		for (int j = BARRIER_THICKNESS + MARGIN; j < BARRIER_THICKNESS + MARGIN + temp; j++)
		{
			map[j][i] = 1;
		}
	}

	// Left Bottom Square
	for (int i = BARRIER_THICKNESS + MARGIN; i < BARRIER_THICKNESS + MARGIN + temp; i++)
	{
		for (int j = BARRIER_THICKNESS + 2*MARGIN + temp; j < Y_MAX - BARRIER_THICKNESS - MARGIN; j++)
		{
			map[j][i] = 1;
		}
	}

	// Right Top Square
	for (int i = BARRIER_THICKNESS + 2*MARGIN +temp; i < X_MAX - BARRIER_THICKNESS - MARGIN; i++)
	{
		for (int j = BARRIER_THICKNESS + MARGIN; j < BARRIER_THICKNESS + MARGIN + temp; j++)
		{
			map[j][i] = 1;
		}
	}

	// Right Bottom Square
	for (int i = BARRIER_THICKNESS + 2*MARGIN +temp; i < X_MAX - BARRIER_THICKNESS - MARGIN; i++)
	{
		for (int j = BARRIER_THICKNESS + 2 * MARGIN + temp; j < Y_MAX - BARRIER_THICKNESS - MARGIN; j++)
		{
			map[j][i] = 1;
		}
	}


	isDrawMap = true;
}

void loadNarrowPassage()
{
	emptyMap();
	loadMapNone();

	// First column of passage
	for (int i = BARRIER_THICKNESS-1; i < BARRIER_THICKNESS + 37; i++)
	{
		map[i][BARRIER_THICKNESS + PASSAGE_MARGIN] = 1;
	}

	for (int i = BARRIER_THICKNESS+37+NARROW_GATE; i <= Y_MAX - BARRIER_THICKNESS; i++)
	{
		map[i][BARRIER_THICKNESS + PASSAGE_MARGIN] = 1;
	}

	//Second column of passage
	for (int i = BARRIER_THICKNESS - 1; i < BARRIER_THICKNESS + 15; i++)
	{
		map[i][X_MAX - BARRIER_THICKNESS - PASSAGE_MARGIN] = 1;
	}

	for (int i = BARRIER_THICKNESS + 15 + LARGE_GATE; i <= Y_MAX - BARRIER_THICKNESS; i++)
	{
		map[i][X_MAX - BARRIER_THICKNESS - PASSAGE_MARGIN] = 1;
	}



	isDrawMap = true;
}

void loadConcave()
{
	emptyMap();
	loadMapNone();

	for (int i = BARRIER_THICKNESS + CONCAVE_MARGIN; i < X_MAX - BARRIER_THICKNESS - CONCAVE_MARGIN; i++)
	{
		map[BARRIER_THICKNESS + CONCAVE_MARGIN][i] = 1;
	}

	for (int i = BARRIER_THICKNESS + CONCAVE_MARGIN; i < X_MAX - BARRIER_THICKNESS - CONCAVE_MARGIN; i++)
	{
		map[BARRIER_THICKNESS + CONCAVE_MARGIN + 12][i] = 1;
	}

	for (int i = BARRIER_THICKNESS + CONCAVE_MARGIN; i < Y_MAX -1 - 2*BARRIER_THICKNESS - CONCAVE_MARGIN; i++)
	{
		map[i][X_MAX - BARRIER_THICKNESS - CONCAVE_MARGIN] = 1;
	}

	isDrawMap = true;
}

void emptyMap()
{
	for (int i = 0; i < X_MAX; i++)
	{
		for (int j = 0; j < Y_MAX; j++)
			map[j][i] = 0;
	}
}

void drawMap()
{
	for (int i = 0; i < Y_MAX; i++)
	{
		for (int j = 0; j < X_MAX; j++)
		{
			if (map[i][j] == 1)
			{
				glColor3f(1.0, 0.0, 0.0);
				glRectd(j, i, j + 1, i + 1);
			}
			else if (map[i][j] == 5)
			{
				glColor3f(1.0, 1.0, 0.0);
				glRectd(j, i, j + 1, i + 1);
			}
			else if (map[i][j] == 6)
			{
				glColor3f(0.0, 1.0, 0.0);
				glRectd(j, i, j + 1, i + 1);
			}
			else if (map[i][j] == 7)
			{
				glColor3f(0.0, 0.9, 0.3);
				glRectd(j, i, j + 1, i + 1);
			}
		}
	}
}
