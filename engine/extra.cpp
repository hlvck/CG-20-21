#include "extra.h"

bool visibleAxis = false;

void axisToggle()
{
    visibleAxis = !visibleAxis;
}

void drawAxis()
{
    if(visibleAxis)
    {
        glBegin(GL_LINES);
        glColor3f(1.0, 0.1, 0.1);
        glVertex3f(-300.0f, 0.0f, 0.0f);
        glVertex3f(300.0f, 0.0f, 0.0f);
        glColor3f(0.1, 1.0, 0.1);
        glVertex3f(0.0f, -300.0f, 0.0f);
        glVertex3f(0.0f, 300.0f, 0.0f);
        glColor3f(0.1, 0.1, 1.0);
        glVertex3f(0.0f, 0.0f, -300.0f);
        glVertex3f(0.0f, 0.0f, 300.0f);
        glColor3f(1,1,1);
        glEnd();
    }
}