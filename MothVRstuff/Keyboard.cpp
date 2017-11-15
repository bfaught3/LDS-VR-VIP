#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#define _SECURE_SCL_DEPRECATE 0
#define _CRT_SECURE_NO_WARNINGS 0

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

void writeShit();

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
bool centering = 0;
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
float aggrlx = 0.0f;

float bias = 0.0;
bool written = 0;

int32       error = 0;
TaskHandle  taskHandle = 0;
int32       read;
float64     data[1400700];
float64		currentData[1400700];
float64		currai0[200100];
float64		currai1[200100];
float64		currai2[200100];
float64		currai3[200100];
float64		currai4[200100];
float64		currai5[200100];
int64		currai6[200100];
float64		currxp[200100];
float64		currxpcl[200100];	// closed loop
int32		queueit = 0;
float64		bias0, bias1, bias2, bias3, bias4, bias5;
/*
int64		ai0it = 0;
int64		ai1it = 0;
int64		ai2it = 0;
int64		ai3it = 0;
int64		ai4it = 0;
int64		ai5it = 0;
*/

char        errBuff[2048] = { '\0' };

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

void writeShit() {
	FILE * fileP = NULL;

	char filename;

	fileP = fopen("test.txt", "w");
	
	if (fileP != NULL) {
		
		for (int i = 0; i < 200100; i++) {
			int helperQueue = queueit + i;
			if (helperQueue >= 200100) {
				helperQueue -= 200100;
			}
			fprintf(fileP, "%f, %f, %f, %f, %f, %f, %f, %f, %i\n", currxp[helperQueue], currxpcl[helperQueue], currai0[helperQueue], currai1[helperQueue], currai2[helperQueue], currai3[helperQueue], currai4[helperQueue], currai5[helperQueue], currai6[helperQueue]);
		}
		fclose(fileP);
	}
	else {
		printf("Our file cannot be written to");
	}




	//printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

}
float64 calcFeedback() { // not finished, also we only need to take the average of what's new in currai0
	float64 avgai0 = 0;
	for (int i = 0; i < read; i++) {
		int32 j = queueit - read + i;
		if (j < 0) {
			avgai0 += currai0[j + 200100];
		}
		else {
			avgai0 += currai0[j];
		}
	}
	avgai0 = avgai0 / read;

	return avgai0;
}

float64 * matrixMult(float64 ai0, float64 ai1, float64 ai2, float64 ai3, float64 ai4, float64 ai5) { // not finished, also we only need to worry about the first line of the calibration matrix to get Fx

	float64 Fx = ((-0.000352378) * ai0) + (0.020472451 * ai1) + ((-0.02633045) * ai2) + ((-0.688977299) * ai3) + (0.000378075 * ai4) + (0.710008955 * ai5);
	float64 Fy = ((-0.019191418) * ai0) + (0.839003543 * ai1) + ((-0.017177775) * ai2) + ((-0.37643613) * ai3) + (0.004482987 * ai4) + ((-0.434163392) * ai5);
	float64 Fz = (0.830046806 * ai0) + (0.004569748 * ai1) + (0.833562339 * ai2) + (0.021075403 * ai3) + (0.802936538 * ai4) + ((-0.001350335) * ai5);
	float64 Tx = ((-0.316303442) * ai0) + (5.061378026 * ai1) + (4.614179159 * ai2) + ((-2.150699522) * ai3) + ((-4.341889297) * ai4) + ((-2.630773662) * ai5);
	float64 Ty = ((-5.320003676) * ai0) + ((-0.156640061) * ai1) + (2.796170871 * ai2) + (4.206523866 * ai3) + (2.780562472 * ai4) + ((-4.252850011) * ai5);
	float64 Tz = ((-0.056240509) * ai0) + (3.091367987 * ai1) + (0.122101875 * ai2) + (2.941467741 * ai3) + (0.005876647 * ai4) + (3.094672928 * ai5);

	Tx = Fy * 94.5 + Tx * 1;
	Ty = Fx * (-94.5) + Ty * 1;
	float64 transformedData[6] = { Fx, Fy, Fz, Tx, Ty, Tz };
	return transformedData;
}

float64 biasing(float64 *readArray) {
	float64 avgai = 0;
	for (int i = 0; i < read; i++) {
		int32 j = queueit - read + i;
		if (j < 0) {
			avgai += readArray[j + 200100];
		}
		else {
			avgai += readArray[j];
		}
	}
	avgai = avgai / read;
	return avgai;
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
	if (currai6[queueit - 1] == 0 && !written) {
		writeShit();
		written = 1;
	}
	if (currai6[queueit - 1] == 1) {
		written = 0;
	}
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
		float64 tempxp = boost * xp;
		aggrlx += lx;
		xp = -sinf(((float)it / (delay/2)) * PI);
		//xp = xp - sinf(((float)it / (400.0)) * PI);
		//if (xp*xp < 0.1)
			//printf("%f", 550 + boost * xp);

		DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, -1, -1.0, DAQmx_Val_GroupByChannel, currentData, 1400700, &read, NULL));
		goto Skip;
	Error:
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
		if (taskHandle != 0) {
			//DAQmxStopTask(taskHandle);
			//DAQmxClearTask(taskHandle);
		}
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);

	Skip:
		for (int i = 0; i < read; i++) {
			if (queueit >= 200100) {
				queueit = 0;
			}
			float64 * tempData = matrixMult((currentData[i] - bias0), (currentData[i + read] - bias1), (currentData[i + (read * 2)] - bias2), (currentData[i + (read * 3)] - bias3), (currentData[i + (read * 4)] - bias4), (currentData[i + (read * 5)] - bias5));
			/*
			currai0[queueit] = currentData[i] - bias0;
			currai1[queueit] = currentData[i + read] - bias1;
			currai2[queueit] = currentData[i + (read * 2)] - bias2;
			currai3[queueit] = currentData[i + (read * 3)] - bias3;
			currai4[queueit] = currentData[i + (read * 4)] - bias4;
			currai5[queueit] = currentData[i + (read * 5)] - bias5;
			currai6[queueit] = currentData[i + (read * 6)];
			*/
			currai0[queueit] = tempData[0];
			currai1[queueit] = tempData[1];
			currai2[queueit] = tempData[2];
			currai3[queueit] = tempData[3];
			currai4[queueit] = tempData[4];
			currai5[queueit] = tempData[5];
			currai6[queueit] = currentData[i + (read * 6)];
			currxp[queueit] = tempxp;
			currxpcl[queueit] = tempxp - aggrlx;
			queueit++;
		}
		//printf("\n%f", currentData[read * 7 - 1]);
		//printf("\n%i", currai6[queueit - 1]);
		/*
		if (currai6[queueit - 1] == 0 && !written) {
			writeShit();
			written = 1;
		}
		if (currai6[queueit - 1] == 1) {
			written = 0;
		}
		*/
		//if (currentData[1] * currentData[1] < 25.0) {
		//	lx = currentData[1] - bias;
		//}
		//else {
		//	lx = 0.0;
		//}
		if (centering) {
			lx = 0;
			centering = 0;
		}
		if (calcFeedback() * calcFeedback() < 5) { // Do something to catch the NaN problem
			lx = calcFeedback();
		}
		//lx = calcFx();

		printf("%f\n", lx);
		//printf("Acquired %d samples\n", (int)read);

		glutPostRedisplay(); //redraws window
		glutTimerFunc(4, speedManager, 0);

	}
	else {
		
		glutPostRedisplay();
		glutTimerFunc(4, speedManager, 0);
	}

}

void key_pressed(int key, int z, int y) {
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
	case 99:
		lx = -aggrlx;
		centering = 1;
		glutPostRedisplay();
		break;
	case 67:
		lx = -aggrlx;
		centering = 1;
		glutPostRedisplay();
		break;
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

	
	/*
	** Add the closed-loop stuff here.
	*/

	// DAQmx analog voltage channel and timing parameters
	DAQmxErrChk(DAQmxCreateTask("", &taskHandle));

	// IMPORTANT
	//changed Dev1 to Dev5 as the connection established. So verify what Dev is being used to update this code. DEV5 is our force torque.
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "Dev5/ai0:6", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", 10000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 200100));
	// DAQmx Start Code
	DAQmxErrChk(DAQmxStartTask(taskHandle));
	printf("\ngot here");
	// DAQmx Read Code
	//for (int it = 0; it < 1000; it++) {
	//printf("%f\n", data[it]);
	//}
	//printf("%f\n", data[0]);
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, 200100, -1.0, DAQmx_Val_GroupByChannel, data, 1400700, &read, NULL));
	printf("%f\n", data[0]);
	printf("\ngot passed through analog");
	printf("%f\n", data[199999]);
	//bias = data[0];
	// Stop and clear task
	printf("Acquired %d samples\n", (int)read);
	//for (int it = 0; it < 200000; it++) {
	//	printf("%f\n", data[it]);
	//}
	//printf("%f\n", data[199999]);
	//printf("Acquired %d samples\n", (int)read);
	//DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, 2, -1.0, DAQmx_Val_GroupByChannel, &data[199998], 2, &read, NULL));
	printf("\ngot passed through analog");
	printf("%f\n", data[1199999]);
	for (int i = 0; i < read; i++) {
		if (queueit >= 200100) {
			queueit = 0;
		}
		currai0[queueit] = data[i];
		currai1[queueit] = data[i + read];
		currai2[queueit] = data[i + (read * 2)];
		currai3[queueit] = data[i + (read * 3)];
		currai4[queueit] = data[i + (read * 4)];
		currai5[queueit] = data[i + (read * 5)];
		currai6[queueit] = data[i + (read * 6)];
		queueit++;
	}
	bias0 = biasing(currai0);
	bias1 = biasing(currai1);
	bias2 = biasing(currai2);
	bias3 = biasing(currai3);
	bias4 = biasing(currai4);
	bias5 = biasing(currai5);
	writeShit();
	/*
Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
	*/

	/*
	** This is where the closed-loop stuff ends
	*/

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
	goto Skip;
Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
		getchar();
Skip:
	glutMainLoop();
	return 0;
}