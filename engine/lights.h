#ifndef ASSESSMENT_LIGHTS_H
#define ASSESSMENT_LIGHTS_H
#endif //ASSESSMENT_LIGHTS_H

#include "tinyxml/tinyxml.h"
#include <GL/glut.h>
#include <vector>

enum LIGHT_TYPE{
    point,
    directional,
    spotlight
};

struct Light {
    LIGHT_TYPE type;
    float ambient[4], specular[4], diffuse[4], pos[4], dir[3];
    explicit Light(LIGHT_TYPE type)
    {
        this->type = type;
        this->ambient[0] = 0.0;
        this->ambient[1] = 0.0;
        this->ambient[2] = 0.0;
        this->ambient[3] = 1.0;
        this->diffuse[0] = 0.0;
        this->diffuse[1] = 0.0;
        this->diffuse[2] = 0.0;
        this->diffuse[3] = 1.0;
        this->specular[0] = 0.0;
        this->specular[1] = 0.0;
        this->specular[2] = 0.0;
        this->specular[3] = 1.0;
        this->pos[0] = 0.0;
        this->pos[1] = 0.0;
        this->pos[2] = 0.0;
        this->pos[3] = (type == directional) ? 0.0 : 1.0;
        this->dir[0] = 0.0;
        this->dir[1] = 0.0;
        this->dir[2] = 0.0;
    }
};

std::vector<Light>* parseLights (TiXmlNode* node);
void drawLights(std::vector<Light>* lights);