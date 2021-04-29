#include "extra.h"

bool visibleAxis = false;
double frames = 0, fps = 0;
int timebase = 0;

void axisToggle()
{
    visibleAxis = !visibleAxis;
}

void displayFps(){
    char title[50];

    frames++;
    long time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000)
    {
        fps = frames*1000/(time-timebase);
        timebase = time;
        frames = 0;
        sprintf(title, "Practical Assignment (%lf FPS)", fps);
        glutSetWindowTitle(title);
    }
};

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