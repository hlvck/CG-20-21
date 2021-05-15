#include "model.h"

GLuint vertex[256];
int modelCounter = 0;
bool enableVBO = true;

std::vector<ModelGroup>* parseXml(char* filename)
{
    TiXmlDocument doc(filename);
    bool success = doc.LoadFile(filename);
    if(!success)
    {
        printf("Error loading XML file. %s. Specify the path as second argument.", doc.ErrorDesc());
        getchar();
        exit(1);
    }
    TiXmlNode* node = doc.FirstChildElement("scene")->FirstChild();
    auto modelGroup = new std::vector<ModelGroup>;
    do {
        modelGroup->emplace_back(*parseGroups(node->FirstChild()));
    } while((node = node->NextSibling()));
    return modelGroup;
}

ModelGroup* parseGroups(TiXmlNode* node)
{
    if(!node) return nullptr;
    auto current = new ModelGroup;

    do {
        if ((!strcmp(node->Value(), "translate")) || (!strcmp(node->Value(), "rotate")) || (!strcmp(node->Value(), "scale")))
        {
            TiXmlAttribute *atrib = node->ToElement()->FirstAttribute();
            auto *transform = new Transform;
            if (!strcmp(node->Value(), "translate")) transform->type = translate;
            else if (!strcmp(node->Value(), "rotate")) transform->type = rotate;
            else if (!strcmp(node->Value(), "scale")) transform->type = scale;
            if(transform->type == scale) {
                transform->x = 1;
                transform->y = 1;
                transform->z = 1;
            }
            do {
                if (!strcmp(atrib->Name(), "X") || !strcmp(atrib->Name(), "axisX")) transform->x = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "Y") || !strcmp(atrib->Name(), "axisY")) transform->y = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "Z") || !strcmp(atrib->Name(), "axisZ")) transform->z = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "angle")) transform->angle = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "time"))
                {
                    transform->time = (float)atrib->DoubleValue();
                    if (transform->type == translate)
                    {
                        transform->x = 0; transform->y = 1; transform->z = 0;
                        TiXmlElement* elem = node->FirstChildElement("point");
                        do {
                            TiXmlAttribute* attr = elem->FirstAttribute();
                            do {
                                if (!strcmp(attr->Name(), "X")) transform->points.emplace_back((float)attr->DoubleValue());
                                if (!strcmp(attr->Name(), "Y")) transform->points.emplace_back((float)attr->DoubleValue());
                                if (!strcmp(attr->Name(), "Z")) transform->points.emplace_back((float)attr->DoubleValue());
                            } while((attr = attr->Next()));
                        } while ((elem = elem->NextSiblingElement()));
                    }
                }
            } while((atrib = atrib->Next()));
            current->transforms.emplace_back(*transform);
        }

        if(!strcmp(node->Value(), "models"))
        {
            TiXmlElement* elem = node->FirstChildElement("model");
            do {
                TiXmlAttribute* attrib = elem->FirstAttribute();
                Model* model;
                do{
                    if(!strcmp(attrib->Name(), "file"))
                    {
                        std::string filename(attrib->Value());
                        model = loadModel(&filename);
                    }
                    else if(!strcmp(attrib->Name(), "specR") && model)
                    {
                        model->specular[0] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "specG") && model)
                    {
                        model->specular[1] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "specB") && model)
                    {
                        model->specular[2] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "ambiR") && model)
                    {
                        model->ambient[0] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "ambiG") && model)
                    {
                        model->ambient[1] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "ambiB") && model)
                    {
                        model->ambient[2] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "diffR") && model)
                    {
                        model->diffuse[0] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "diffG") && model)
                    {
                        model->diffuse[1] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "diffB") && model)
                    {
                        model->diffuse[2] = attrib->DoubleValue();
                    }
                    else if(!strcmp(attrib->Name(), "shine") && model)
                    {
                        model->shininess = attrib->DoubleValue();
                    }
                } while((attrib = attrib->Next()));
                if(model) current->models.emplace_back(*model);
            } while((elem = elem->NextSiblingElement()));
        }

        if(!strcmp(node->Value(), "group"))
        {
            auto child = parseGroups(node->FirstChild());
            if(!current->children) current->children = new std::vector<ModelGroup>;
            current->children->emplace_back(*child);
        }
    } while((node = node->NextSibling()));
    return current;
}

void initializeVBO (std::vector<float> vertices, std::vector<float> normals, int modelIndex)
{
    //initialize, copy to gpu
    glGenBuffers(2, &vertex[modelIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex[modelIndex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertex[modelIndex+1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), normals.data(), GL_STATIC_DRAW);
}

Model* loadModel (std::string* filename)
{
    std::ifstream file(*filename);
    std::string line;
    std::vector<float> vertices, normals;
    if(!file.is_open())
    {
        std::cout << "Error opening file: " << *filename << std::endl;
        return nullptr;
    }

    std::stringstream ss;
    getline(file, line);
    int numPoints = stoi(line);
    int i = 0;

    while(getline(file, line))
    {
        ss.clear();
        ss.str(line);
        while(ss.good()){
            std::string substr;
            getline(ss, substr, ',');
            if (i < numPoints) {
                vertices.emplace_back(stof(substr));
            } else normals.emplace_back(stof(substr));
        }
        i++;
    }

    initializeVBO(vertices, normals, modelCounter);
    auto mod = new Model(vertices, normals, modelCounter);
    modelCounter+=2;
    return mod;
}

void drawModels(std::vector<ModelGroup>* modelgroups)
{
    for (auto &group : *modelgroups)
    {
        glPushMatrix();
        for(auto &transform : group.transforms)
        {
            switch (transform.type)
            {
                case translate:
                    if(transform.time > 0 && transform.points.size() > 3){
                        float prevY[3] = {transform.x, transform.y, transform.z};
                        transform.gt = catmullRom(transform.gt, transform.time, transform.points.data(), transform.points.size()/3, prevY);
                        transform.x = prevY[0]; transform.y = prevY[1]; transform.z = prevY[2];
                    } else {
                        glTranslatef(transform.x, transform.y, transform.z);
                    }
                    break;
                case rotate:
                    if(transform.time > 0)
                    {
                        float spf = 0;
                        if (fps) spf = 1/fps;
                        float rotv = (spf/transform.time)*360;
                        transform.angle = (transform.angle + rotv) < 360 ? transform.angle+rotv : 0;
                    }
                    glRotatef(transform.angle, transform.x, transform.y, transform.z);
                    break;
                case scale:
                    glScalef(transform.x, transform.y, transform.z);
                    break;
            }
        }
        for (auto model : group.models)
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT, model.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, model.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, model.specular);
            glMaterialf(GL_FRONT, GL_SHININESS, model.shininess);
            if(enableVBO)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vertex[model.modelIndex]);
                glVertexPointer(3, GL_FLOAT, 0,0);
                glBindBuffer(GL_ARRAY_BUFFER,vertex[model.modelIndex+1]);
                glNormalPointer(GL_FLOAT, 0, 0);

                glDrawArrays(GL_TRIANGLES, 0, model.vertices.size());
            } else {
                glBegin(GL_TRIANGLES);
                for(int i = 0; i < model.vertices.size(); i+=3)
                {
                    glVertex3f(model.vertices[i], model.vertices[i+1], model.vertices[i+2]);
                }
                glEnd();
            }
        }
        if(group.children) (drawModels(group.children));
        glPopMatrix();
    }
}

void VBOToggle()
{
    enableVBO = !enableVBO;
}