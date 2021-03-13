#define _USE_MATH_DEFINES
#ifndef ASSESSMENT_MODEL_H
#define ASSESSMENT_MODEL_H
#endif //ASSESSMENT_MODEL_H
#include "tinyxml/tinyxml.h"
#include <GL/glut.h>

struct Point {
    float x, y, z;
};

struct Model {
    int numPoints;
    Point* vertices;
};

Model** parseXml(char*);
Model** loadModels(char**, int);
Point* addPoint(Point*, float, float, float);
void drawModels(Model**);