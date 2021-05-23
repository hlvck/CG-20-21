#define _USE_MATH_DEFINES
#ifndef ASSESSMENT_MODEL_H
#define ASSESSMENT_MODEL_H
#endif //ASSESSMENT_MODEL_H
#include "tinyxml/tinyxml.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "extra.h"
#include "catmullrom.h"

enum TRANSF_TYPE{
    translate,
    rotate,
    scale
};

struct Transform {
    TRANSF_TYPE type;
    float angle, x, y, z, time, gt;
    std::vector<float> points;

    Transform()
    {
        this->type = translate;
        this->angle = 0;
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->time = 0;
        this->gt = 0;
    }
};

struct Model {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    int modelIndex;
    float ambient[4], specular[4], diffuse[4], shininess;

    explicit Model(std::vector<float> points, std::vector<float> npoints, std::vector<float> texture, int index)
    {
        this->vertices = std::move(points);
        this->normals = std::move(npoints);
        this->texCoords = std::move(texture);
        this->modelIndex = index;
        this->ambient[0] = 0.2;
        this->ambient[1] = 0.2;
        this->ambient[2] = 0.2;
        this->ambient[3] = 1.0;
        this->diffuse[0] = 0.8;
        this->diffuse[1] = 0.8;
        this->diffuse[2] = 0.8;
        this->diffuse[3] = 1.0;
        this->specular[0] = 0;
        this->specular[1] = 0;
        this->specular[2] = 0;
        this->specular[3] = 1.0;
        this->shininess = 64.0;
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