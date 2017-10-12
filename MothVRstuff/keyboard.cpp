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
int it = 0, it1 = 0; //iterator for iterator
int mod = 20;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors
	glColor3f(255, 255, 255); // color white for our rectangle

	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex2i(800 + xp, 0 + yp);
	glVertex2i(750 + xp, 0 + yp);
	glVertex2i(750 + xp, 800 + yp);
	glVertex2i(800 + xp, 800 + yp);

	//while (1) {
	it1++;
	printf("%d", it);
	if (it1 % mod == 0) {
		it++;
		if (it % 1500 < 750) {
			xp--;
		}
		else {
			xp++;
		}
		printf("We are stuck here forevermore");
		printf("\n");
		glutPostRedisplay(); //redraws window
	}
	else {
		//it++;
		glutPostRedisplay();
	}

	//glutPostRedisplay();
	printf("loop back to stuff");
	printf("\n");
	glEnd();
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
}

void key_pressed(int key, int x, int y) {
	//xp++;
	switch (key) {
	case GLUT_KEY_RIGHT:
		//xp--;
		//it++;
		if (mod > 1) {
			mod--;
		}
		//else {
		//	xp++;
		//}
		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_LEFT:
		//xp++;
		//it++;
		mod++;
		glutPostRedisplay(); //redraws window
		break;
	}
}

void oscillate(int key, int i, int x) {
	switch (key) {
	case GLUT_KEY_LEFT:
		while (key == GLUT_KEY_LEFT) {
			int it = 0;
			if (it % 20 < 10) {
				xp--;
				glutPostRedisplay();
			}
			else {
				xp++;
				glutPostRedisplay();
			}
			it++;
		}
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
	//glutSpecialFunc(oscillate);

	printf("Where it's hitting");
	printf("\n");
	glutMainLoop();
	return 0;
}