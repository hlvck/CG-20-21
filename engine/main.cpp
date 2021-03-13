#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "model.h"
#include "tinyxml/tinyxml.h"

float alpha = 0, beta = M_PI/4;
float dist = 10;

char** att = parseXml("../demo/scene.xml");

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
    /*float radius = 1.2;
    float height = 3;
    int slices = 10;
    glBegin(GL_TRIANGLES);

    float red=1;
    for(int i = 0; i < slices; i++)
    {
        double alpha = (i*2*M_PI)/slices;
        double secalpha = ((i+1)*2*M_PI)/slices;
        glColor3f(0,i*red/slices,i*red/slices);
        glVertex3f(radius*sin(alpha),0,radius*cos(alpha));
        glVertex3f(radius*sin(secalpha),0,radius*cos(secalpha));
        glVertex3f(radius*sin(alpha),height,radius*cos(alpha));

        glVertex3f(radius*sin(alpha),height,radius*cos(alpha));
        glVertex3f(radius*sin(secalpha),0,radius*cos(secalpha));
        glVertex3f(radius*sin(secalpha),height,radius*cos(secalpha));

        glVertex3f(radius*sin(alpha),0,radius*cos(alpha)); //bot
        glVertex3f(0,0,0);
        glVertex3f(radius*sin(secalpha),0,radius*cos(secalpha));

        glVertex3f(radius*sin(alpha),height,radius*cos(alpha)); //top
        glVertex3f(radius*sin(secalpha),height,radius*cos(secalpha));
        glVertex3f(0,height,0);
    }
    glEnd();
*/
    glBegin(GL_TRIANGLES);
    double xlen = 4;
    double ylen = 3;
    double zlen = 2;
    int divisions = 10;
    for(int i = 0; i<divisions; i++)
    {
        for(int j = 0; j < divisions; j++)
        {
            double x1 = (j*xlen)/divisions;
            double x2 = ((j+1)*xlen)/divisions;
            double y1 = (i*ylen)/divisions;
            double y2 = ((i+1)*ylen)/divisions;
            double z1 = (j*zlen)/divisions;
            double z2 = ((j+1)*zlen)/divisions;
            double x1tb = (i*xlen)/divisions;
            double x2tb = ((i+1)*xlen)/divisions;

            glVertex3f(x1, y1, zlen);           //faces by x, z
            glVertex3f(x2, y1, zlen);
            glVertex3f(x1, y2, zlen);
            glVertex3f(x1, y2, zlen);
            glVertex3f(x2, y1, zlen);
            glVertex3f(x2, y2, zlen);

            glVertex3f(x1, y1, 0);           //faces by x, 0
            glVertex3f(x1, y2, 0);
            glVertex3f(x2, y2, 0);
            glVertex3f(x2, y2, 0);
            glVertex3f(x2, y1, 0);
            glVertex3f(x1, y1, 0);

            glVertex3f(0, y1, z1);           //faces by z, 0
            glVertex3f(0, y1, z2);
            glVertex3f(0, y2, z1);
            glVertex3f(0, y2, z1);
            glVertex3f(0, y1, z2);
            glVertex3f(0, y2, z2);

            glVertex3f(xlen, y1, z1);           //faces by z, x
            glVertex3f(xlen, y2, z1);
            glVertex3f(xlen, y2, z2);
            glVertex3f(xlen, y2, z2);
            glVertex3f(xlen, y1, z2);
            glVertex3f(xlen, y1, z1);

            glVertex3f(x1tb, ylen, z1);           // top
            glVertex3f(x1tb, ylen, z2);
            glVertex3f(x2tb, ylen, z2);
            glVertex3f(x2tb, ylen, z2);
            glVertex3f(x2tb, ylen, z1);
            glVertex3f(x1tb, ylen, z1);

            glVertex3f(x1tb, 0, z1);           // bot
            glVertex3f(x2tb, 0, z1);
            glVertex3f(x1tb, 0, z2);
            glVertex3f(x1tb, 0, z2);
            glVertex3f(x2tb, 0, z1);
            glVertex3f(x2tb, 0, z2);
        }
    }
    glEnd();

	// End of frame
	glutSwapBuffers();
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
        case '1':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case '2':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }
    if(key == 'w' && dist >= 0.16) dist -= 0.16;
    if(key == 's') dist += 0.16;

    glutPostRedisplay();
}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
    glutSpecialFunc(keyboardSpecial);
    glutKeyboardFunc(keyboard);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
