#ifndef AUXILIARY_FUNCTIONS_INCLUDED_H
#define AUXILIARY_FUNCTIONS_INCLUDED_H

#define SAMPLING_STEP	0.1

float mapValue(float val, float fromMin, float fromMax, float toMin, float toMax);
bool isPathFree(int stX, int stY, int glX, int glY);
bool isInFreeSpace(int x, int y);

#endif // !AUXILIARY_FUNCTIONS_INCLUDED_H
