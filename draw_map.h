#ifndef DRAW_MAP_H_INCLUDED
#define DRAW_MAP_H_INCLUDED

#define BARRIER_THICKNESS	2
#define MARGIN				8
#define PASSAGE_MARGIN		20
#define NARROW_GATE			4
#define LARGE_GATE			8
#define CONCAVE_MARGIN		20

void loadMapNone();
void loadMapSparse();
void loadNarrowPassage();
void loadConcave();
void emptyMap();
void drawMap();
void visualizeNodes();
void visualizeConnectionsBetweenNodes();
void drawLine(int x1, int y1, int x2, int y2, float r, float g, float b, float lineThickness);
void visualizeFinalPath();

#endif // !DRAW_MAP_H_INCLUDED
