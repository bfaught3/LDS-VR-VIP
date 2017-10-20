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
#include <time.h>       /* time_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

float xp = 0, yp = 0;
int it = 0, it1 = 0; //iterator for iterator
int mod = 1;
int delay = 10;
int boost = 7;
bool clear = 0;
static unsigned int fps_start = 0;
static unsigned int fps_frames = 0;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors
	glColor3f(255, 255, 255); // color white for our rectangle
	//time_t start, end;
	
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex2i(600 + boost * xp, 0 + yp);
	glVertex2i(550 + boost * xp, 0 + yp);
	glVertex2i(550 + boost * xp, 800 + yp);
	glVertex2i(600 + boost * xp, 800 + yp);



	glEnd();
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
}

void billybob(int bob) {
	
	//printf("in billybob\n");
	fps_frames++;
	//std::cout << "\n" << glutGet(GLUT_ELAPSED_TIME);
	int delta_t = glutGet(GLUT_ELAPSED_TIME) - fps_start;
	if (delta_t > 1000) {
		std::cout << double(delta_t) / double(fps_frames) << std::endl;
		fps_frames = 0;
		fps_start = glutGet(GLUT_ELAPSED_TIME);
	}
	//while (1) {
	it1++;
	//printf("%d\n", it1);
	if (it1 % mod == 0) {
		it1 = 0;
		it++;
		if (it % 700 == 0) {
			//time_t end;
			//end = time(0);
			//printf("%d", end);
			//printf("Display func took this amount of time : %0.6d \n", difftime(start, end));
			it = 0;

		}
		else if (it % 350 == 0) {

		}
		if (it % (700 / boost) < (350 / boost)) {
			xp--;
		}
		else {
			xp++;
		}
		//printf("We are stuck here forevermore\n");
		//printf("\n");
		glutPostRedisplay(); //redraws window
		glutTimerFunc(delay, billybob, 0);
	}
	else {
		//it++;
		//printf("We are stuck here forevermore again\n");
		glutPostRedisplay();
		glutTimerFunc(delay, billybob, 0);
	}

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

void letter_pressed(unsigned char key, int x, int y) {
	switch (key) {
	case 98:
		if (clear) {
			yp = 0;
			clear = false;
		}
		else {
			yp = -800;
			clear = true;
		}
		glutPostRedisplay();
		break;
	case 66:
		if (clear) {
			yp = 0;
			clear = false;
		}
		else {
			yp = -800;
			clear = true;
		}
		glutPostRedisplay();
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


	fps_start = glutGet(GLUT_ELAPSED_TIME);
	//glutTimerFunc(1000, billybob, 0);
	glutDisplayFunc(display);
	//printf("finished display\n");
	glutTimerFunc(1, billybob, 0);
	//printf("finished billybob\n");

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0.0, 800, 0.0, 800);
	glutSpecialFunc(key_pressed);
	glutKeyboardFunc(letter_pressed);

	//printf("Where it's hitting");
	printf("\n");
	glutMainLoop();
	glutTimerFunc(1, billybob, 0);
	return 0;
}