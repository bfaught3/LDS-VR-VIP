#include <iostream>
//#include "..\..\..\Libs and Include\Include\glut.h"

#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
//#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

float xp = 0, yp = 0;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors
	glColor3f(255, 255, 255); // color white for our rectangle

	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex2i(600 + xp, 200 + yp);
	glVertex2i(500 + xp, 200 + yp);
	glVertex2i(500 + xp, 300 + yp);
	glVertex2i(600 + xp, 300 + yp);

	glEnd();
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
}

void key_pressed(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		xp--;
		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_RIGHT:
		xp++;
		glutPostRedisplay(); //redraws window
		break;
	}
}

int main(int argc, char** argv) {
	printf("Press left or right arrows to move our rectangle");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Testing OpenGL");

	glutDisplayFunc(display);
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0.0, 800, 0.0, 800);
	glutSpecialFunc(key_pressed);

	glutMainLoop();
	return 0;
}