#include "model.h"
#include <fstream>
#define MAX_MODELS 64

int num_models = 0;

Model** parseXml(char* filename)
{
    TiXmlDocument doc(filename);
    bool success = doc.LoadFile(filename);
    char* modelnames[MAX_MODELS];
    int i = 0;

    if(!success)
    {
        printf("Error loading XML file. %s", doc.ErrorDesc());
        getchar();
        exit(1);
    }
    TiXmlElement* model = doc.FirstChildElement("scene")->FirstChildElement("model");
    while(model)
    {
        modelnames[i] = strdup(model->Attribute("file"));
        printf("Model name: %s\n", modelnames[i]);
        i++;
        model = model->NextSiblingElement();
    }
    Model** models = loadModels(modelnames, i);
    num_models = i;
    return models;
}

Model** loadModels (char** modelnames, int nummodels)
{
    Model** models = (Model**)malloc(sizeof(Model*)*nummodels);
    std::ifstream modelfile;
    for(int i = 0; i < nummodels; i++){
        modelfile.open(modelnames[i]);
        std::string line;
        if(!modelfile.is_open())
        {
            printf("Error opening file: %s.\n", modelnames[i]);
            continue;
        }
        getline(modelfile, line);
        models[i] = (Model*)malloc(sizeof(Model));
        models[i]->numPoints = atoi(line.c_str());
        models[i]->vertices = (Point*)malloc(sizeof(Point)*models[i]->numPoints);
        int j = 0;
        float x,y,z;
        while(getline(modelfile, line))
        {
            x = stof(line.substr(0, line.find(',')), NULL);
            line.erase(0, line.find(",")+1);
            y = stof(line.substr(0, line.find(',')), NULL);
            line.erase(0, line.find(",")+1);
            z = stof(line.substr(0, line.find(',')), NULL);
            line.erase(0, line.find(",")+1);
            addPoint(&models[i]->vertices[j], x, y, z);
            j++;
        }
        modelfile.close();
    }
    return models;
}

Point* addPoint(Point* p, float x, float y, float z)
{
    p->x = x;
    p->y = y;
    p->z = z;
    return p;
}

void drawModels(Model** models)
{
    int i = 0;
    while(i < num_models)
    {
        glBegin(GL_TRIANGLES);
        for(int j = 0; j < models[i]->numPoints; j+=3)
        {
            glVertex3f(models[i]->vertices[j].x, models[i]->vertices[j].y, models[i]->vertices[j].z);
            glVertex3f(models[i]->vertices[j+1].x, models[i]->vertices[j+1].y, models[i]->vertices[j+1].z);
            glVertex3f(models[i]->vertices[j+2].x, models[i]->vertices[j+2].y, models[i]->vertices[j+2].z);
        }
        glEnd();
        i++;
    }
}