#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#define _SECURE_SCL_DEPRECATE 0
#define _CRT_SECURE_NO_WARNINGS 0
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#define PI 3.14159265
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
#define DAQmx_Val_GroupByChannel 0

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
#include <time.h>



/*
 * Global variable helpers
 **/
static unsigned int fps_start = 0;
static unsigned int fps_frames = 0;
int it = 0, it1 = 0; //iterator for iterator
int delayIt = 3;
int xAccelIt = 3;
int increment = 0;
static int num = 550, num2 = 600, num3 = 800;
float delayArr[4] = { 125.0, 250.0, 500.0, 2500.0 };
// delay is being casted to an int from float double check if this is behavior thats wanted.
int delay = (int)delayArr[delayIt];
float xp = 0, yp = 0;
float boost = 175.0;
const float weight = 0.00202f;		// This is the moth's weight
const float width = 1167.883212f;
const float threshold = 0.0025;
float xAccelArr[5] = { -0.5f, -0.1f, 0.0f, 0.1f, 0.5f};
float xAccel = xAccelArr[xAccelIt];
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 0.0f;
float aggrlx = 0.0f;
bool clear = 0;
bool centering = 0;
bool written = 0;
// for NIDAQ data handling
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

char        errBuff[2048] = { '\0' };

bool rebias = 1;
/*
 * OpenGL function for drawing our bar
 */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors

	glColor3f(255, 255, 0); // color white for our rectangle is (255, 255, 255); color yellow is (255, 255, 0)

	/*
	if (centering) {
		gluLookAt(0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, -1.0f,
				  0.0f, 1.0f, 0.0f);
	*/
	//} else {
	if (((xp - (aggrlx + lx) < -325) && (lx > 0)) || ((xp - (aggrlx + lx) > 325) && (lx < 0))) {	// This is supposed to keep the bar on the screen
		lx = 0;
	}
	gluLookAt(x + lx, 0.0f, z,
			  x + lx, 0.0f, z + lz,
			  0.0f, 1.0f, 0.0f);
	//}
	aggrlx += lx;
	//printf("\n%d", x + lx);
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(475 + xp, 0 + yp, 0);	//475
	glVertex3f(325 + xp, 0 + yp, 0);	//325
	glVertex3f(325 + xp, 800 + yp, 0);
	glVertex3f(475 + xp, 800 + yp, 0);

	glEnd();
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
}

/*
 * Allows for the user to printout data obtain from NIDAQ channels.
 * Currently will start outputting to file after trigger has been pressed.
 **/
void writeToFile() {
	FILE * fileP = NULL;

	time_t rawtime;
	struct tm *info;
	char buffer[80];

	time(&rawtime);
	info = localtime(&rawtime);
	strftime(buffer, 80, "%m%d%Y", info);

	char filename[80];
	sprintf(filename, "./Data/Moth_FT_%s_%03d.txt", buffer, increment);

	fileP = fopen(filename, "w");
	printf("\n%s", filename);

	if (fileP != NULL) {
		for (int i = 0; i < 200100; i++) {
			int helperQueue = queueit + i;
			if (helperQueue >= 200100) {
				helperQueue -= 200100;
			}
			fprintf(fileP, "%f, %f, %f, %f, %f, %f, %f, %f, %lld\n", currxp[helperQueue], currxpcl[helperQueue], currai0[helperQueue], currai1[helperQueue], currai2[helperQueue], currai3[helperQueue], currai4[helperQueue], currai5[helperQueue], currai6[helperQueue]);
		}
		fclose(fileP);
		increment++;
	}
	else {
		printf("\nOur file cannot be written to");
	}

}

/*
 *  Calculates the force along the x-axis, averaged over the most recent samples. The units are in Newtons.
 **/
float64 calcFeedback() {
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

	return -avgai0;		// This is negative because the force transducer is backwards.
}

/*
 * Allows for the conversion of voltages into force and torque.
 **/
 float64 * matrixMult(float64 ai0, float64 ai1, float64 ai2, float64 ai3, float64 ai4, float64 ai5) { 

	float64 Fx = ((-0.000352378) * ai0) + (0.020472451 * ai1) + ((-0.02633045) * ai2) + ((-0.688977299) * ai3) + (0.000378075 * ai4) + (0.710008955 * ai5);
	float64 Fy = ((-0.019191418) * ai0) + (0.839003543 * ai1) + ((-0.017177775) * ai2) + ((-0.37643613) * ai3) + (0.004482987 * ai4) + ((-0.434163392) * ai5);
	float64 Fz = (0.830046806 * ai0) + (0.004569748 * ai1) + (0.833562339 * ai2) + (0.021075403 * ai3) + (0.802936538 * ai4) + ((-0.001350335) * ai5);
	float64 Tx = ((-0.316303442) * ai0) + (5.061378026 * ai1) + (4.614179159 * ai2) + ((-2.150699522) * ai3) + ((-4.341889297) * ai4) + ((-2.630773662) * ai5);
	float64 Ty = ((-5.320003676) * ai0) + ((-0.156640061) * ai1) + (2.796170871 * ai2) + (4.206523866 * ai3) + (2.780562472 * ai4) + ((-4.252850011) * ai5);
	float64 Tz = ((-0.056240509) * ai0) + (3.091367987 * ai1) + (0.122101875 * ai2) + (2.941467741 * ai3) + (0.005876647 * ai4) + (3.094672928 * ai5);
	
	Tx = Tx + 94.5 * Fy;
	Ty = Ty - 94.5 * Fx;

	float64 transformedData[6] = { Fx, Fy, Fz, Tx, Ty, Tz };
	return transformedData;
}

float64 biasing(float64 *readArray) {
	float64 avgai = 0;
	int ignore = 0;
	for (int i = ignore; i < read; i++) {
		int32 j = queueit - read + i;
		if (j < 0) {
			avgai += readArray[j + 200100];
		}
		else {
			avgai += readArray[j];
		}
	}
	avgai = avgai / (read - ignore);
	return avgai;
}
/*
 * This function allows to change the speed of the bar
 * @param speed - value parameter given by the glutTimerFunc
 */
void speedManager(int speed) {
	//printf("\n%f", lx);
	//printf("\n%i",centering);
	fps_frames++;
	int delta_t = glutGet(GLUT_ELAPSED_TIME) - fps_start;
	if (delta_t > 1000) {
		//std::cout << double(delta_t) / double(fps_frames) << std::endl;
		fps_frames = 0;
		fps_start = glutGet(GLUT_ELAPSED_TIME);
	}

	if (currai6[queueit - 1] == 0 && !written) {
		writeToFile();
		written = 1;
	}
	if (currai6[queueit - 1] == 1) {
		written = 0;
	}
	it1++;

	if (it1 == 1) {
		it1 = 0;
		it++;
		if (it >= 2500) {
			it = 0;
		}

		float64 tempxp = xp;
		//aggrlx += lx;
		//aggrlx += lx * (double(delta_t) / double(fps_frames))c / 1000;
		xp = -sinf(((float)it / (float)(delay / 2)) * (float)PI) * boost;

		DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, -1, -1.0, DAQmx_Val_GroupByChannel, currentData, 1400700, &read, NULL));
		goto Skip;

	Error:
		if (DAQmxFailed(error)) {
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
			printf("\nDAQmx Error: %s\n", errBuff);
		}

	Skip:
		for (int i = 0; i < read; i++) {
			if (queueit >= 200100) {
				queueit = 0;
			}
			//printf("%f\n", currentData[i]);
		    float64 * tempData = matrixMult((currentData[i] - bias0), (currentData[i + read] - bias1), (currentData[i + (read * 2)] - bias2), (currentData[i + (read * 3)] - bias3), (currentData[i + (read * 4)] - bias4), (currentData[i + (read * 5)] - bias5));
			currai0[queueit] = tempData[0];
			//printf("%f", tempData[0]);
			currai1[queueit] = tempData[1];
			currai2[queueit] = tempData[2];
			currai3[queueit] = tempData[3];
			currai4[queueit] = tempData[4];
			currai5[queueit] = tempData[5];
			currai6[queueit] = (int64)currentData[i + (read * 6)];
			currxp[queueit] = tempxp;
			currxpcl[queueit] = tempxp - aggrlx;
			queueit++;
		}
		
		if (centering) {
			lx = 0;
			centering = 0;
		}
		else if (calcFeedback() * calcFeedback() < 5 && abs(calcFeedback()) > threshold) { // Do something to catch the NaN problem
			//lx = calcFeedback();
			//lx += calcFeedback()/moth's weight in kg * 1574.80315 pixels/m * read * 1.0/10000.0 * 1.0/250.0;
			lx += (float)(calcFeedback() / (weight)) * width * read * (1.0f / 10000.0f) * (1.0f / 250.0f);
		}
		//aggrlx += lx;
		//aggrlx += lx * (double(delta_t) / double(fps_frames)) / 1000;
		//printf("%f\n", lx);
		//printf("Acquired %d samples\n", (int)read);

		glutPostRedisplay(); //redraws window
		glutTimerFunc(4, speedManager, 0);

	}
	else {
		glutPostRedisplay();
		glutTimerFunc(4, speedManager, 0);
	}

}

/*
 * Takes in various key inputs.
 * Deals more with the movement of our bar.
 **/
void key_pressed(int key, int z, int y) {
	float fraction = 0.1f;
	switch (key) {
	case GLUT_KEY_UP:

		if (delayIt > 0) {
			delayIt--;
			delay = (int)delayArr[delayIt];
		}
		glutPostRedisplay(); //redraws window
		break;
	case GLUT_KEY_DOWN:

		if (delayIt < 3) {
			delayIt++;
			delay = (int)delayArr[delayIt];
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


/*
 * Callback function to retrieve key value movement
 * @param key - ASCII value for key input 
 * @param x -  n/a
 * @param y - n/a
 */
void letter_pressed(unsigned char key, int x, int y) { // Does various things when various letters are pressed; x and y are irrelevant.
	switch (key) {
	case 98: //letter b
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
	case 66: //letter B
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
	case 99: // letter C
		//aggrlx += lx;
		lx = -aggrlx;
		it = 0;
		centering = 1; //iffy
		glutPostRedisplay();
		break;
	case 67: //letter C
		//aggrlx += lx;
		lx = -aggrlx;
		it = 0;
		centering = 1;
		glutPostRedisplay();
		break;
	case 82: //letter R
		if (rebias) {
			bias0 += biasing(currai0);
			bias1 += biasing(currai1);
			bias2 += biasing(currai2);
			bias3 += biasing(currai3);
			bias4 += biasing(currai4);
			bias5 += biasing(currai5);
			rebias = 0;
			glutPostRedisplay();
		}

		break;
	case 114: //letter r
		if (rebias) {
			bias0 += biasing(currai0);
			bias1 += biasing(currai1);
			bias2 += biasing(currai2);
			bias3 += biasing(currai3);
			bias4 += biasing(currai4);
			bias5 += biasing(currai5);
			rebias = 0;
			glutPostRedisplay();
		}
		break;
	case 69:
		rebias = 1;
		break;
	case 101:
		rebias = 1;
		break;
	case 27: // ESC to exit fullscreen
		exit(0);
		break;
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
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai0:6", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", 10000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 200100));
	// DAQmx Start Code
	DAQmxErrChk(DAQmxStartTask(taskHandle));
	//printf("\ngot here");
	// DAQmx Read Code
	//printf("%f\n", data[0]);
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, 200100, -1.0, DAQmx_Val_GroupByChannel, data, 1400700, &read, NULL));
	//printf("%f\n", data[0]);
	//printf("\ngot passed through analog");
	//printf("%f\n", data[199999]);
	//bias = data[0];
	// Stop and clear task
	printf("Acquired %d samples\n", (int)read);
	//printf("%f\n", data[199999]);
	//printf("Acquired %d samples\n", (int)read);
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
		currai6[queueit] = (int64)data[i + (read * 6)]; //our trigger buttom. Must be floored to 0 or 1.
		queueit++;
	}
	bias0 = biasing(currai0);
	bias1 = biasing(currai1);
	bias2 = biasing(currai2);
	bias3 = biasing(currai3);
	bias4 = biasing(currai4);
	bias5 = biasing(currai5);
	writeToFile();

	/*
	** This is where the closed-loop stuff ends
	*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Virtual Reality - Closed Loop");


	fps_start = glutGet(GLUT_ELAPSED_TIME);
	glutFullScreen();
	glutDisplayFunc(display);
	//printf("finished display\n");
	glutTimerFunc(0, speedManager, 0);
	//printf("finished speedManager\n");

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0.0, 800, 0.0, 800);

	glutSpecialFunc(key_pressed);
	glutKeyboardFunc(letter_pressed);
	//printf("\n Will do something when the keys are hit");
	goto Skip;
Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		printf("\nDAQmx Error: %s\n", errBuff);
	getchar();
Skip:
	glutMainLoop();
	return 0;
}