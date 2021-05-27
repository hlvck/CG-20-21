//
// Created by andre on 25/05/2021.
//

#include "lights.h"

std::vector<Light>* parseLights (TiXmlNode* node)
{
    auto lights = new std::vector<Light>;
    if(!node) return nullptr;
    Light* current;
    TiXmlElement* elem = node->FirstChildElement("light"); if(!elem) return nullptr;
    do {
        TiXmlAttribute* attrib = elem->FirstAttribute();
        if(!strcmp(attrib->Value(), "POINT"))
        {
            current = new Light(point);
        } else if (!strcmp(attrib->Value(), "SPOTLIGHT") || !strcmp(attrib->Value(), "SPOT"))
        {
            current = new Light(spotlight);
        } else if (!strcmp(attrib->Value(), "DIRECTIONAL"))
        {
            current = new Light(directional);
        } else return nullptr;

        while((attrib = attrib->Next())) {
            if(!strcmp(attrib->Name(), "posX") && current)
            {
                current->pos[0] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "posY") && current)
            {
                current->pos[1] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "posZ") && current)
            {
                current->pos[2] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "dirX") && current)
            {
                current->dir[0] = attrib->DoubleValue();
                if(current->type == directional) current->pos[0] = current->dir[0];
            }
            else if(!strcmp(attrib->Name(), "dirY") && current)
            {
                current->dir[1] = attrib->DoubleValue();
                if(current->type == directional) current->pos[1] = current->dir[1];
            }
            else if(!strcmp(attrib->Name(), "dirZ") && current)
            {
                current->dir[2] = attrib->DoubleValue();
                if(current->type == directional) current->pos[2] = current->dir[2];
            }
            else if(!strcmp(attrib->Name(), "specR") && current)
            {
                current->specular[0] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "specG") && current)
            {
                current->specular[1] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "specB") && current)
            {
                current->specular[2] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "ambiR") && current)
            {
                current->ambient[0] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "ambiG") && current)
            {
                current->ambient[1] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "ambiB") && current)
            {
                current->ambient[2] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "diffR") && current)
            {
                current->diffuse[0] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "diffG") && current)
            {
                current->diffuse[1] = attrib->DoubleValue();
            }
            else if(!strcmp(attrib->Name(), "diffB") && current)
            {
                current->diffuse[2] = attrib->DoubleValue();
            }
        }
        lights->emplace_back(*current);
    } while((elem = elem->NextSiblingElement()));
    return lights;
}

void drawLights(std::vector<Light>* plights)
{
    if (plights == nullptr) return;
    std::vector<Light> lights = *plights;
    for (int i = 0; i < lights.size(); i++)
    {
        GLenum lightId = GL_LIGHT0 + i;
        glEnable(lightId);
        glLightfv(lightId, GL_AMBIENT, lights[i].ambient);
        glLightfv(lightId, GL_DIFFUSE, lights[i].diffuse);
        glLightfv(lightId, GL_SPECULAR, lights[i].specular);
        glLightfv(lightId, GL_POSITION, lights[i].pos);
        if(lights[i].type == spotlight) {
            glLightf(lightId, GL_SPOT_CUTOFF, 90);
            glLightf(lightId, GL_SPOT_EXPONENT, 25);
            glLightfv(lightId, GL_SPOT_DIRECTION, lights[i].dir);
            glLightf(lightId, GL_LINEAR_ATTENUATION, 0.2);
        }
    }
}
