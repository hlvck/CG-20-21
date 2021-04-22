#define _USE_MATH_DEFINES
#ifndef ASSESSMENT_MODEL_H
#define ASSESSMENT_MODEL_H
#endif //ASSESSMENT_MODEL_H
#include "tinyxml/tinyxml.h"
#include <GL/glew.h>
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

    Transform()
    {
        this->type = translate;
        this->angle = 0;
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
};

struct Model {
    std::vector<float> vertices;
    int modelIndex;

    explicit Model(std::vector<float> points, int index)
    {
        this->vertices = std::move(points);
        this->modelIndex = index;
    }

};

struct ModelGroup {
    std::vector<Transform> transforms;
    std::vector<Model> models;
    std::vector<ModelGroup>* children;

    ModelGroup()
    {
        this->children = nullptr;
    }
};

std::vector<ModelGroup>* parseXml(char*);
ModelGroup* parseGroups(TiXmlNode*);
Model* loadModel(std::string*);
void drawModels(std::vector<ModelGroup>*);
void VBOToggle();