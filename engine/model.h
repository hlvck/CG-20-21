#define _USE_MATH_DEFINES
#ifndef ASSESSMENT_MODEL_H
#define ASSESSMENT_MODEL_H
#endif //ASSESSMENT_MODEL_H
#include "tinyxml/tinyxml.h"
#include <GL/glut.h>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum TRANSF_TYPE{
    translate,
    rotate,
    scale
};

struct Transform {
    TRANSF_TYPE type;
    float angle, x, y, z;
};

struct Model {
    std::vector<float> vertices;

    Model(std::vector<float> points)
    {
        vertices = points;
    }

};

struct ModelGroup {
    std::vector<Transform> transforms;
    std::vector<Model> models;
    std::vector<ModelGroup>* children;
};

std::vector<ModelGroup>* parseXml(char*);
ModelGroup* parseGroups(TiXmlNode*);
Model* loadModel(std::string);
//Model** loadModels(char**, int);
void drawModels(Model**);