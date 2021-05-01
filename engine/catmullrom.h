#ifndef ASSESSMENT_CATMULLROM_H
#define ASSESSMENT_CATMULLROM_H
#endif //ASSESSMENT_CATMULLROM_H

#include <GL/glut.h>
#include <cmath>
#include "extra.h"

float catmullRom(float t, float tTotal, float* points, int size, float* prevY);
void toggleCurve();