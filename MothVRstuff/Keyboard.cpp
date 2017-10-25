#include <iostream>

#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut_std.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

float xp = 0, yp = 0;
int it = 0, it1 = 0; //iterator for iterator
int mod = 1;
int delayArr[4] = { 5, 10, 20, 100 };
int delayIt = 0;
int delay = delayArr[delayIt];
int boost = 7;
bool clear = 0;
static unsigned int fps_start = 0;
static unsigned int fps_frames = 0;
static int num = 550, num2 = 600, num3 = 800;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors
	glColor3f(255, 255, 255); // color white for our rectangle
	
	glBegin(GL_QUADS);  //Rectangle drawing
// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex2i(600 + boost * xp, 0 + yp);
	glVertex2i(550 + boost * xp, 0 + yp);
	glVertex2i(550 + boost * xp, 800 + yp);
	glVertex2i(600 + boost * xp, 800 + yp);

	glEnd();
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
}

void speedManager(int speed) {
	
	int delta_t = glutGet(GLUT_ELAPSED_TIME) - fps_start;
	if (delta_t > 1000) {
		std::cout << double(delta_t) / double(fps_frames) << std::endl;
		fps_frames = 0;
		fps_start = glutGet(GLUT_ELAPSED_TIME);
	}
	
	it1++;
	
	if (it1 == mod) {
		it1 = 0;
		it++;
		
		if (it >= (700 / boost)) {
			it = 0;
		}
		if (it < (350 / boost)) {
			xp--;
		}
		else {
			xp++;
		}
		
		glutPostRedisplay(); //redraws window
		glutTimerFunc(delay, speedManager, 0);
	}
	else {
		
		glutPostRedisplay();
		glutTimerFunc(delay, speedManager, 0);
	}

}

void key_pressed(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_RIGHT:

		if (delayIt > 0) {
			delayIt--;
			delay = delayArr[delayIt];
		}
		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_LEFT:
		
		if (delayIt < 3) {
			delayIt++;
			delay = delayArr[delayIt];
		}
	
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
	case 114:
		glPushMatrix();
		glTranslatef(200, 300, 0);
		glRotatef(15, 0, 0, 1);
		glBegin(GL_QUADS);
		{
			glVertex2f(-num / 2, -num2 / 2);
			glVertex2f(-num2 / 2, -num2 / 2);
			glVertex2f(num2 / 2, num3 / 2);
			glVertex2f(num / 2, num3/ 2);
		}
		glEnd();
		glPopMatrix();
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
	//glutTimerFunc(1000, speedManager, 0);
	glutDisplayFunc(display);
	//printf("finished display\n");
	glutTimerFunc(0, speedManager, 0);
	//printf("finished speedManager\n");

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0.0, 800, 0.0, 800);
	glutSpecialFunc(key_pressed);
	glutKeyboardFunc(letter_pressed);

	printf("\n");
	glutMainLoop();
	//glutTimerFunc(1, billybob, 0);
	return 0;
}
