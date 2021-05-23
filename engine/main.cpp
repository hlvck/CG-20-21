#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include "model.h"
#include "extra.h"
#include "tinyxml/tinyxml.h"

float alpha = 0, beta = M_PI/4;
float dist = 20;
std::vector<ModelGroup>* models;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {

    float posZ = dist * cos(beta) * cos(alpha);
    float posX = dist * cos(beta) * sin(alpha);
    float posY = dist * sin(beta);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
	glLoadIdentity();
	gluLookAt(posX,posY,posZ,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

    // put the geometric transformations here


    // put drawing instructions here
    drawAxis();
    drawModels(models);

    displayFps();
	// End of frame
	glutSwapBuffers();
	glutPostRedisplay();
}



// write function to process keyboard events
void keyboardSpecial(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT:
            alpha -= (2*M_PI)/32;
            break;
        case GLUT_KEY_RIGHT:
            alpha += (2*M_PI)/32;
            break;
        case GLUT_KEY_UP:
            if((beta+(2*M_PI)/32) < M_PI/2) beta += (2*M_PI)/32;
            break;
        case GLUT_KEY_DOWN:
            if((beta-(2*M_PI)/32) > -M_PI/2) beta -= (2*M_PI)/32;
            break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
            if(dist >= 0.16) dist -= 0.16;
            break;
        case 's':
            dist += 0.16;
            break;
        case 'l':
            axisToggle();
            break;
        case 'k':
            toggleCurve();
            break;
        case 'v':
            VBOToggle();
            break;
        case '1':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case '2':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }

    glutPostRedisplay();
}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Practical Assignment");

		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
    glutSpecialFunc(keyboardSpecial);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    glewInit();
#endif

//  OpenGL settings
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

//  Load models
    if(argc >= 2) {
        models = parseXml(argv[1]);
    } else {
        models = parseXml("../demo/scene2.xml");
    }

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
