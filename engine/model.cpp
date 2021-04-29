#include "model.h"

GLuint vertex[128];
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
                        TiXmlElement* elem = node->FirstChildElement("point");
                        do {
                            TiXmlAttribute* attr = elem->FirstAttribute();
                            do {
                                if (!strcmp(attr->Name(), "X")) transform->points.emplace_back((float)attr->DoubleValue());
                                if (!strcmp(attr->Name(), "Y")) transform->points.emplace_back((float)attr->DoubleValue());
                                if (!strcmp(attr->Name(), "Z")) transform->points.emplace_back((float)attr->DoubleValue());
                            } while((attr = attr->Next()));
                            //transform->points.emplace_back(point);
                        } while ((elem = elem->NextSiblingElement()));
                    }
                }
            } while((atrib = atrib->Next()));
            current->transforms.emplace_back(*transform);
        }

        if(!strcmp(node->Value(), "models"))
        {
            std::vector<std::string> filenames;
            TiXmlElement* elem = node->FirstChildElement("model");
            do {
                filenames.emplace_back(elem->Attribute("file"));
            } while((elem = elem->NextSiblingElement()));

            for(auto file : filenames)
            {
                Model* model = loadModel(&file);
                if(model) current->models.emplace_back(*model);
            }
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

void initializeVBO (std::vector<float> vertices, int modelIndex)
{
    //initialize, copy to gpu
    glGenBuffers(1, &vertex[modelIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex[modelIndex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
}

Model* loadModel (std::string* filename)
{
    std::ifstream file(*filename);
    std::string line;
    std::vector<float> vertices;
    if(!file.is_open())
    {
        std::cout << "Error opening file: " << *filename << std::endl;
        return nullptr;
    }

    std::stringstream ss;
    getline(file, line);

    while(getline(file, line))
    {
        ss.clear();
        ss.str(line);
        while(ss.good()){
            std::string substr;
            getline(ss, substr, ',');
            vertices.emplace_back(stof(substr));
        }
    }

    initializeVBO(vertices, modelCounter);
    auto mod = new Model(vertices, modelCounter);
    modelCounter++;
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
                    glTranslatef(transform.x, transform.y, transform.z);
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
            if(enableVBO)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vertex[model.modelIndex]);
                glVertexPointer(3, GL_FLOAT, 0,0);
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