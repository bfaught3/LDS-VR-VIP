#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

// OpenGL is pretty senstive so our header file glut.h has to be the first on our include files
#include <GL/glut.h> // MUST BE FIRST (not including MACROS. Those are fine first.)
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <time.h>       /* time_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
#include <iostream>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <NIDAQmx.h>
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#define PI 3.14159265
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

float xp = 0, yp = 0;
int it = 0, it1 = 0, it2 = 0; //iterator for iterator
int mod = 1;
//int delayArr[4] = { 5, 10, 20, 100 };
float delayArr[4] = { 125.0, 250.0, 500.0, 2500.0 };
int delayIt = 0;
int delay = delayArr[delayIt];
//double boost = 6.4;
float boost = 175.0;
//int boost = 4;
bool clear = 0;
static unsigned int fps_start = 0;
static unsigned int fps_frames = 0;
static int num = 550, num2 = 600, num3 = 800;
float xAccelArr[5] = { -0.5, -0.1, 0.0, 0.1, 0.5 };
int xAccelIt = 3;
float xAccel = xAccelArr[xAccelIt];

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 0.0f;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors
														//glLoadIdentity();

														//gluLookAt(400.0f, 400.0f, 100.0f,
														//	400.0f, 400.0f, 0.0f,
														//	400.0f, 401.0f, 0.0f);

														//glMatrixMode(GL_PROJECTION);
														//glLoadIdentity();
														//gluPerspective(60.0, width / (float)height, 0.1, 100.0);
														//gluPerspective(60.0, 1.0, 0.1, 100.0);
														//glMatrixMode(GL_MODELVIEW);

														//gluLookAt(0.0f, 0.0f, 0.0f,
														//	0.0f, 0.0f, -1.0f,
														//	0.0f, 1.0f, 0.0f);

														//glViewport(0, 0, 800, 800);

	glColor3f(255, 255, 255); // color white for our rectangle

	gluLookAt(x + lx, 0.0f, z,
		x + lx, 0.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
						//glVertex3f(400 + boost * xp, 0 + yp, 0);
						//glVertex3f(550 + boost * xp, 0 + yp, 0);
						//glVertex3f(550 + boost * xp, 800 + yp, 0);
						//glVertex3f(600 + boost * xp, 800 + yp, 0);
						//glVertex3f(600 +  xp, 0 + yp, 0);
						//glVertex3f(550 +  xp, 0 + yp, 0);
						//glVertex3f(550 +  xp, 800 + yp, 0);
						//glVertex3f(600 +  xp, 800 + yp, 0);
	glVertex3f(425 + boost * xp, 0 + yp, 0);
	glVertex3f(375 + boost * xp, 0 + yp, 0);
	glVertex3f(375 + boost * xp, 800 + yp, 0);
	glVertex3f(425 + boost * xp, 800 + yp, 0);
	//printf("%f\n", lx);

	glEnd();
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
}

/*
** This function allows to change the speed of the bar
*/
void speedManager(int speed) {

	fps_frames++;
	int delta_t = glutGet(GLUT_ELAPSED_TIME) - fps_start;
	if (delta_t > 1000) {
		//std::cout << double(delta_t) / double(fps_frames) << std::endl;
		fps_frames = 0;
		fps_start = glutGet(GLUT_ELAPSED_TIME);
	}

	/*
	** Add the closed-loop stuff here.


	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	float64     data[200000];
	char        errBuff[2048] = { '\0' };
	// DAQmx analog voltage channel and timing parameters
	DAQmxErrChk(DAQmxCreateTask("", &taskHandle));

	// IMPORTANT
	//changed Dev1 to Dev5 as the connection established. So verify what Dev is being used to update this code. DEV5 is our force torque.
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "Dev5/ai0", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", 10000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 200000));
	// DAQmx Start Code
	DAQmxErrChk(DAQmxStartTask(taskHandle));
	// DAQmx Read Code
	//for (int it = 0; it < 1000; it++) {
	//printf("%f\n", data[it]);
	//}
	//printf("%f\n", data[0]);
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, 200000, -1.0, DAQmx_Val_GroupByChannel, data, 200000, &read, NULL));
	// Stop and clear task
	printf("Acquired %d samples\n", (int)read);
	for (int it = 0; it < 2; it++) {
	printf("%f\n", data[it]);
	}
	//printf("Acquired %d samples\n", (int)read);
	Error:
	if (DAQmxFailed(error))
	DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
	printf("DAQmx Error: %s\n", errBuff);

	/*
	** This is where the closed-loop stuff ends
	*/

	//gluLookAt(0.0f, 0.0f, 10.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f);
	it1++;

	if (it1 == 1) {
		it1 = 0;
		it++;

		//if (it > (double(800 / boost))) {
		if (it >= 2500) {
			//if (it > 800) {
			//if (it >= (2000 / boost)) {
			it = 0;
		}
		/*
		if (it < (double(400 / boost))) {
		//if (it < 62) {
		xp--;
		}
		else if (it < (double(800 / boost))) {
		xp++;
		}
		else if (it = (double(800 / boost))) {
		//xp++;
		it = 0;
		}
		*/
		//xp = xp - sinf(((float)it / (1256.0 / boost)) * PI);
		xp = -sinf(((float)it / (delay / 2)) * PI);
		//xp = xp - sinf(((float)it / (400.0)) * PI);
		//if (xp*xp < 0.1)
		//printf("%f", 550 + boost * xp);

		//if (currentData[1] * currentData[1] < 25.0) {
		//	lx = currentData[1] - bias;
		//}
		//else {
		//	lx = 0.0;
		//}
		
		//lx = calcFx();

		//printf("Acquired %d samples\n", (int)read);

		glutPostRedisplay(); //redraws window
		glutTimerFunc(4, speedManager, 0);

	}
	else {

		glutPostRedisplay();
		glutTimerFunc(4, speedManager, 0);
	}

}

void key_pressed(int key, int x, int y) {
	float fraction = 0.1f;
	switch (key) {
	case GLUT_KEY_UP:

		if (delayIt > 0) {
			delayIt--;
			delay = delayArr[delayIt];
		}
		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_DOWN:

		if (delayIt < 3) {
			delayIt++;
			delay = delayArr[delayIt];
		}

		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_LEFT:
		if (xAccelIt > 0) {
			xAccelIt--;
			xAccel = xAccelArr[xAccelIt];
		}
		lx = xAccel;
		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_RIGHT:
		if (xAccelIt < 4) {
			xAccelIt++;
			xAccel = xAccelArr[xAccelIt];
		}
		lx = xAccel;
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
		//Not implemented yet
	case 114:
		printf("We are entering our switch case\n");
		glPushMatrix();
		glTranslatef(200, 300, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		{
			glVertex2f(-num / 2, -num2 / 2);
			glVertex2f(-num2 / 2, -num2 / 2);
			glVertex2f(num2 / 2, num3 / 2);
			glVertex2f(num / 2, num3 / 2);
		}
		glEnd();
		glPopMatrix();
	}
}

int main(int argc, char** argv) {
	printf("Press left or right arrows to move our rectangle");

	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	//glutInitWindowSize(1600, 800);
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
	//gluOrtho2D(0.0, 1600, 0.0, 800);

	glutSpecialFunc(key_pressed);
	glutKeyboardFunc(letter_pressed);
	printf("\n Will do something when the keys are hit");

	//printf("\n");
	glutMainLoop();
	return 0;
}
