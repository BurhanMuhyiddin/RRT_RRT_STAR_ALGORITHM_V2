#include <iostream>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <stdlib.h> 
#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include "window_parameters.h"
#include "menu_parameters.h"
#include "draw_map.h"
#include "auxiliary_functions.h"
#include "rrt_algorithm.h"

using namespace std;

// Callback functions
void display_callback();
void reshape_callback(int, int);
void menu_callback(int);
void mouse_callback(int, int, int, int);
void keyboard_callback(unsigned char, int, int);
void timer_callback(int);
// End of callback functions

// User-defined functions
void initMenu();
// End of user-defined functions

// User-defined variables
bool isDrawMap = false;
bool is_set_goal = false;
bool is_set_init = false;
bool is_start_to_solve_rrt = false;
bool isSolveRRT = false;
bool isSolveRRT_Star = false;
bool is_mission_accomplished = false;
int sub1, sub2, returnmenu;
float goalX = 999, goalY = 999;
float startX = 999, startY = 999;
// End of user-defined variables

extern unsigned int map[Y_MAX][X_MAX];
extern vector < vector<int> > nodes;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initMenu();
	srand(time(NULL));
	initRRT();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(W_W, W_H);
	glutCreateWindow("RRT_RRT*");

	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutKeyboardFunc(keyboard_callback);
	glutMouseFunc(mouse_callback);

	glutTimerFunc(0, timer_callback, 0);

	init();

	glutMainLoop();
}

void initMenu()
{
	sub1 = glutCreateMenu(menu_callback);
	glutAddMenuEntry("none", NONE);
	glutAddMenuEntry("sparse", SPARSE);
	glutAddMenuEntry("narrow passage", NARROW_PASSAGE);
	glutAddMenuEntry("concave", CONCAVE);

	sub2 = glutCreateMenu(menu_callback);
	glutAddMenuEntry("rrt", RRT);
	glutAddMenuEntry("rrt*", RRT_STAR);

	returnmenu = glutCreateMenu(menu_callback);
	glutAddSubMenu("map", sub1);
	glutAddSubMenu("algorithm", sub2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display_callback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (isDrawMap)
	{
		drawMap();

		if (is_start_to_solve_rrt && !is_mission_accomplished)
		{
			is_mission_accomplished = solveRRT();
			//drawParentMap();
		}

		visualizeNodes();
		visualizeConnectionsBetweenNodes();
	}

	if (is_mission_accomplished)
	{
		visualizeFinalPath();
	}

	glutSwapBuffers();
}

void reshape_callback(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, X_MAX, Y_MAX, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void menu_callback(int item)
{
	switch (item)
	{
		case NONE:
			loadMapNone();
			break;
		case SPARSE:
			loadMapSparse();
			break;
		case NARROW_PASSAGE:
			loadNarrowPassage();
			break;
		case CONCAVE:
			loadConcave();
			break;
		case RRT:
			isSolveRRT = true;
			break;
		case RRT_STAR:
			isSolveRRT_Star = true;
			break;
	}
}

void keyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'i':	// Start draw mode
		is_set_goal = false;
		is_set_init = true;
		break;
	case 'g':	// Goal draw mode
		is_set_init = false;
		is_set_goal = true;
		break;
	case 's':
		is_set_init = false;
		is_set_goal = false;
		if(isSolveRRT)	is_start_to_solve_rrt = true;
	}
}

void mouse_callback(int button, int state, int mouseX, int mouseY)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (is_set_init)
		{
			is_set_init = false;

			if(startX < X_MAX && startY < Y_MAX)
				map[(int)startY][(int)startX] = 0;

			float tempX = mapValue(mouseX, 0, W_W, 0, X_MAX);
			float tempY = mapValue(mouseY, 0, W_H, 0, Y_MAX);

			if (map[(int)tempY][(int)tempX] != 1)
			{
				startX = tempX;
				startY = tempY;
			}

			vector <int> temp;
			temp.push_back((int)startX);
			temp.push_back((int)startY);
			nodes.push_back(temp);

			map[(int)startY][(int)startX] = 5;
		}
		else if (is_set_goal)
		{
			is_set_goal = false;

			if (goalX < X_MAX && goalY < Y_MAX)
				map[(int)goalY][(int)goalX] = 0;

			float tempX = mapValue(mouseX, 0, W_W, 0, X_MAX);
			float tempY = mapValue(mouseY, 0, W_H, 0, Y_MAX);

			if (map[(int)tempY][(int)tempX] != 1)
			{
				goalX = tempX;
				goalY = tempY;
			}

			int glX = (int)goalX; int glY = (int)goalY;

			map[glY][glX] = 6;

			for (int i = 0; i < R; i++)
			{
				int j = i + 1;
				map[glY - j][glX - j] = 7;
				map[glY - j][glX] = 7;
				map[glY - j][glX + j] = 7;
				map[glY][glX + j] = 7;
				map[glY][glX - j] = 7;
				map[glY + j][glX - j] = 7;
				map[glY + j][glX] = 7;
				map[glY + j][glX + j] = 7;
			}
		}
	}
}

void timer_callback(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer_callback, 0);
}