#include "model.h"


std::vector<ModelGroup>* parseXml(char* filename)
{
    TiXmlDocument doc(filename);
    bool success = doc.LoadFile(filename);
    if(!success)
    {
        printf("Error loading XML file. %s", doc.ErrorDesc());
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
    auto current = new ModelGroup;
    if(!node) return nullptr;

    do {
        if ((!strcmp(node->Value(), "translate")) || (!strcmp(node->Value(), "rotate")) || (!strcmp(node->Value(), "scale")))
        {
            TiXmlAttribute *atrib = node->ToElement()->FirstAttribute();
            auto *transform = new Transform;
            if (!strcmp(node->Value(), "translate")) transform->type = translate;
            else if (!strcmp(node->Value(), "rotate")) transform->type = rotate;
            else if (!strcmp(node->Value(), "scale")) transform->type = scale;
            transform->x = 0;
            transform->y = 0;
            transform->z = 0;
            transform->angle = 0;
            do {
                float val;
                if (!strcmp(atrib->Name(), "X") || !strcmp(atrib->Name(), "axisX")) transform->x = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "Y") || !strcmp(atrib->Name(), "axisY")) transform->y = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "Z") || !strcmp(atrib->Name(), "axisZ")) transform->z = (float)atrib->DoubleValue();
                if (!strcmp(atrib->Name(), "angle")) transform->angle = (float)atrib->DoubleValue();
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
                Model* model = loadModel(file);
                if(model) current->models.emplace_back(*model);
            }
        }

        if(!strcmp(node->Value(), "group"))
        {
            //auto child = new ModelGroup;
            auto child = parseGroups(node->FirstChild());
            if(!current->children) current->children = new std::vector<ModelGroup>;
            current->children->emplace_back(*child);
        }
    } while((node = node->NextSibling()));
    return current;
}

Model* loadModel (std::string filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<float> vertices;
    if(!file.is_open())
    {
        std::cout << "Error opening file: " << filename << std::endl;
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
   return new Model(vertices);
}

void drawModels(std::vector<ModelGroup>* modelgroups)
{
    for (auto group : *modelgroups)
    {
        glPushMatrix();
        for(auto transform : group.transforms)
        {
            switch (transform.type)
            {
                case translate:
                    glTranslatef(transform.x, transform.y, transform.z);
                    break;
                case rotate:
                    glRotatef(transform.angle, transform.x, transform.y, transform.z);
                    break;
                case scale:
                    glScalef(transform.x, transform.y, transform.z);
                    break;
            }
        }
        for (auto model : group.models)
        {
            glBegin(GL_TRIANGLES);
            for(int i = 0; i < model.vertices.size(); i+=3)
            {
                glVertex3f(model.vertices[i], model.vertices[i+1], model.vertices[i+2]);
            }
            glEnd();
        }
        if(group.children) (drawModels(group.children));
        glPopMatrix();
    }
}