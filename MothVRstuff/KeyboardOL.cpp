/*
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
#include <time.h>       // time_t, clock, CLOCKS_PER_SEC
#include <math.h>       // sqrt 
#include <iostream>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <NIDAQmx.h>
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#define PI 3.14159265
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
*/

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
#include <windows.h>
#include "wglext.h"
//#include <string.h>
//#include <GL/glui.h>

float xp = 0, yp = 0;
int it = 0, it1 = 0, it2 = 0; //iterator for iterator
int mod = 1;
//int delayArr[4] = { 5, 10, 20, 100 };
//float delayArr[4] = { 125.0, 250.0, 500.0, 2500.0 };
float delayArr[4] = { 60.0, 120.0, 240.0, 1200.0 };
int delayIt = 3;
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
int increment = 0;
int barwidthIt = 1;
//float barwidthArr[3] = { 145.8f / 2, 221.5f / 2, 300.9f / 2 };
float barwidthArr[3] = { 72, 110, 150 }; //Half of the barwidth, not the whole thing.
float barwidth = barwidthArr[barwidthIt];
int bars[15] = { -28, -24, -20, -16, -12, -8, -4, 0, 4, 8, 12, 16, 20, 24, 28 };

bool isRight[15] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true }; //This keeps track of the rightmost bar
bool isLeft[15] = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false }; //This keeps track of the leftmost bar

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 0.0f;
float aggrlx = 0.0f;

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
bool written = 0;

GLfloat vertices0[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices1[] = { 400 + (-4 + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
400 + (-4 - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
400 + (-4 - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
400 + (-4 + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v3

400 + (-4 + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
400 + (-4 + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
400 + (-4 + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
400 + (-4 + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,  1, 1, 1,              // v5

400 + (-4 + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
400 + (-4 + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,   1, 1, 1,              // v5
400 + (-4 - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
400 + (-4 - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

400 + (-4 - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
400 + (-4 - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,  1, 1, 1,              // v6
400 + (-4 - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
400 + (-4 - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

400 + (-4 - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v7 (bottom)
400 + (-4 + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
400 + (-4 + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
400 + (-4 - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

400 + (-4 + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
400 + (-4 - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
400 + (-4 - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,  1, 1, 1,              // v6
400 + (-4 + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,  1, 1, 1 };            // v5
GLfloat vertices2[] = { 400 + (0 + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
400 + (0 - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
400 + (0 - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,  1, 1, 1,              // v2
400 + (0 + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v3

400 + (0 + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
400 + (0 + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,  1, 1, 1,              // v3
400 + (0 + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
400 + (0 + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,              // v5

400 + (0 + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
400 + (0 + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
400 + (0 - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
400 + (0 - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

400 + (0 - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
400 + (0 - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
400 + (0 - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,  1, 1, 1,              // v7
400 + (0 - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,  1, 1, 1,              // v2

400 + (0 - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v7 (bottom)
400 + (0 + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v4
400 + (0 + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
400 + (0 - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

400 + (0 + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
400 + (0 - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
400 + (0 - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
400 + (0 + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
GLfloat vertices3[] = { 400 + (4 + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
400 + (4 - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
400 + (4 - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v2
400 + (4 + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v3

400 + (4 + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
400 + (4 + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
400 + (4 + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
400 + (4 + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,              // v5

400 + (4 + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
400 + (4 + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,   1, 1, 1,              // v5
400 + (4 - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,  1, 1, 1,              // v6
400 + (4 - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

400 + (4 - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
400 + (4 - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
400 + (4 - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
400 + (4 - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

400 + (4 - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v7 (bottom)
400 + (4 + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
400 + (4 + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,             // v3
400 + (4 - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

400 + (4 + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
400 + (4 - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
400 + (4 - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
400 + (4 + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
GLfloat vertices4[] = { 400 + (8 + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
400 + (8 - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
400 + (8 - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
400 + (8 + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

400 + (8 + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
400 + (8 + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
400 + (8 + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
400 + (8 + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

400 + (8 + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
400 + (8 + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
400 + (8 - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
400 + (8 - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

400 + (8 - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
400 + (8 - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
400 + (8 - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
400 + (8 - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

400 + (8 - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
400 + (8 + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
400 + (8 + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
400 + (8 - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

400 + (8 + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
400 + (8 - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
400 + (8 - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
400 + (8 + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
GLfloat vertices5[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices6[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices7[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices8[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices9[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices10[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices11[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices12[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices13[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
GLfloat vertices14[] = { 400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

400 + (-8 + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

400 + (-8 - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
400 + (-8 + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
400 + (-8 - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

400 + (-8 + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
400 + (-8 - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
400 + (-8 - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
400 + (-8 + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5

// index array of vertex array for glDrawElements() & glDrawRangeElement()
GLubyte indices[] = { 0, 1, 2,   2, 3, 0,      // front
					4, 5, 6,   6, 7, 4,      // right
					8, 9,10,  10,11, 8,      // top
					12,13,14,  14,15,12,      // left
					16,17,18,  18,19,16,      // bottom
					20,21,22,  22,23,20 };    // back

///////////////////////////////////////////////////////////////////////////////
// draw cube at bottom-left corner with glDrawElements and interleave array
// All the vertex data (position, normal, colour) can be placed together into a 
// single array, and be interleaved like (VNCVNC...). The interleave vertex data
// provides better memory locality.
// Since we are using a single interleaved vertex array to store vertex
// positions, normals and colours, we need to specify "stride" and "pointer"
// parameters properly for glVertexPointer, glNormalPointer and glColorPointer.
// Each vertex has 9 elements of floats (3 position + 3 normal + 3 color), so,
// the stride param should be 36 (= 9 * 4 bytes).
///////////////////////////////////////////////////////////////////////////////
void draw0()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices0 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices0 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices0);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw1()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices1 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices1 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices1);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw2()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices2 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices2 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices2);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw3()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices3 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices3 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices3);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw4()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices4 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices4 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices4);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw5()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices5 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices5 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices5);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw6()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices6 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices6 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices6);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw7()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices7 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices7 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices7);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw8()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices8 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices8 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices8);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw9()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices9 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices9 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices9);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw10()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices10 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices10 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices10);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw11()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices11 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices11 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices11);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw12()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices12 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices12 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices12);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw13()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices13 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices13 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices13);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw14()
{
	// enable and specify pointers to vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 9 * sizeof(GLfloat), vertices14 + 3);
	glColorPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices14 + 6);
	glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertices14);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colors
	//glClear(GL_COLOR_BUFFER_BIT);
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

	aggrlx += lx;

	for (int i = 0; i < 15; i++) {
		if (isLeft[i] && (400 + (bars[i] - 1) * barwidth + xp) >= aggrlx) { //When the leftmost edge of the leftmost bar is within the screen, freak out (aka send the rightmost bar to the left)
			bars[(i + 14) % 15] -= 60;
			isLeft[(i + 14) % 15] = true;
			isLeft[i] = false;
			isRight[(i + 14) % 15] = false;
			isRight[(i + 13) % 15] = true;
		}
		else if (isRight[i] && (400 + (bars[i] + 1) * barwidth + xp) <= aggrlx + 800) {
			bars[(i + 1) % 15] += 60;
			isRight[(i + 1) % 15] = true;
			isRight[i] = false;
			isLeft[(i + 1) % 15] = false;
			isLeft[(i + 2) % 15] = true;
		}
	}

	int barTemp = bars[0];
	/*
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/
	
	GLfloat verticesTemp0[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0, 0, 0, 1, 1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0, 0, 0, 1, 1, 1, 1,              // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0, 1, 0, 0, 1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0, 1, 0, 0, 1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10, 1, 0, 0, 1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10, 1, 0, 0, 1, 1, 1,              // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10, 0, 1, 0, 1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0, 0, 1, 0, 1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0, -1, 0, 0, 1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10, -1, 0, 0, 1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10, -1, 0, 0, 1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0, -1, 0, 0, 1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10, 0, -1, 0, 1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0, 0, -1, 0, 1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10, 0, 0, -1, 1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10, 0, 0, -1, 1, 1, 1 };            // v5
	memcpy(vertices0, verticesTemp0, sizeof(verticesTemp0));

	barTemp = bars[1];
	
	/*
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/
	
	GLfloat verticesTemp1[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,  1, 1, 1,              // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,   1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,  1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,  1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,  1, 1, 1 };            // v5
	memcpy(vertices1, verticesTemp1, sizeof(verticesTemp1));

	barTemp = bars[2];
	
	/*
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/
	
	GLfloat verticesTemp2[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,  1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,  1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,              // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,  1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,  1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices2, verticesTemp2, sizeof(verticesTemp2));

	barTemp = bars[3];
	
	/*
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/
	
	GLfloat verticesTemp3[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,              // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,   1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,  1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,  1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,             // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices3, verticesTemp3, sizeof(verticesTemp3));

	barTemp = bars[4];
	
	/*
	glBegin(GL_QUADS);  //Rectangle drawing
						// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/
	
	GLfloat verticesTemp4[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices4, verticesTemp4, sizeof(verticesTemp4));
	
	barTemp = bars[5];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp5[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices5, verticesTemp5, sizeof(verticesTemp5));

	barTemp = bars[6];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp6[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices6, verticesTemp6, sizeof(verticesTemp6));

	barTemp = bars[7];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp7[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices7, verticesTemp7, sizeof(verticesTemp7));

	barTemp = bars[8];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp8[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices8, verticesTemp8, sizeof(verticesTemp8));

	barTemp = bars[9];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp9[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices9, verticesTemp9, sizeof(verticesTemp9));

	barTemp = bars[10];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp10[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices10, verticesTemp10, sizeof(verticesTemp10));

	barTemp = bars[11];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp11[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices11, verticesTemp11, sizeof(verticesTemp11));

	barTemp = bars[12];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp12[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices12, verticesTemp12, sizeof(verticesTemp12));

	barTemp = bars[13];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp13[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices13, verticesTemp13, sizeof(verticesTemp13));

	barTemp = bars[14];

	/*
	glBegin(GL_QUADS);  //Rectangle drawing
	// Will be using xp and yp as our changing x-position and y-position in our window
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0);	//475
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0);	//325
	glVertex3f(400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0);
	glVertex3f(400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0);

	glEnd();
	*/

	GLfloat verticesTemp14[216] = { 400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v0 (front)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 0, 1,   1, 1, 1,              // v1
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,              // v2
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0, 0, 1,   1, 1, 1,             // v3

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   1, 0, 0,   1, 1, 1,              // v0 (right)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   1, 0, 0,   1, 1, 1,              // v3
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   1, 0, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   1, 0, 0,   1, 1, 1,             // v5

		400 + (barTemp + 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v0 (top)
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,  0, 1, 0,    1, 1, 1,              // v5
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 1, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,   0, 1, 0,   1, 1, 1,              // v1

		400 + (barTemp - 1) * barwidth + xp, 800 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v1 (left)
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v6
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,  -1, 0, 0,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,  -1, 0, 0,   1, 1, 1,              // v2

		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v7 (bottom)
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0,-1, 0,   1, 1, 1,              // v4
		400 + (barTemp + 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v3
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, 0,   0,-1, 0,   1, 1, 1,              // v2

		400 + (barTemp + 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v4 (back)
		400 + (barTemp - 1) * barwidth + xp, 0 + yp, -10,   0, 0,-1,   1, 1, 1,              // v7
		400 + (barTemp - 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1,              // v6
		400 + (barTemp + 1) * barwidth + xp, 800 + yp, -10,   0, 0,-1,   1, 1, 1 };            // v5
	memcpy(vertices14, verticesTemp14, sizeof(verticesTemp14));
	//printf("%f\n", lx);

	//glEnd();
	
	draw0();
	draw1();
	draw2();
	draw3();
	draw4();
	draw5();
	draw6();
	draw7();
	draw8();
	draw9();
	draw10();
	draw11();
	draw12();
	draw13();
	draw14();
	
	glutSwapBuffers(); //done with current frame. Swap to being on the next.
	//glFinish();
}



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


bool WGLExtensionSupported(const char *extension_name)
{
	// this is pointer to function which returns pointer to string with list of all wgl extensions
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

	// determine pointer to wglGetExtensionsStringEXT function
	_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	if (_wglGetExtensionsStringEXT == 0 ||
		(_wglGetExtensionsStringEXT == (void*)0x1) || (_wglGetExtensionsStringEXT == (void*)0x2) || (_wglGetExtensionsStringEXT == (void*)0x3) ||
		(_wglGetExtensionsStringEXT == (void*)-1))
	{
		int a = 23;
		HMODULE module = LoadLibraryA("opengl32.dll");
		_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)GetProcAddress(module, "wglGetExtensionsStringEXT");
	}

	if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
	{
		// string was not found
		printf("Extension not supported");
		return false;
	}

	// extension is supported
	printf("Extension supported");
	return true;
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
** This function allows to change the speed of the bar
*/
//void speedManager(int speed) {
void speedManager() {

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

		//if (it > (double(800 / boost))) {
		//if (it >= 2500) {
		if (it >= delayArr[3]) {
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
		//xp = -sinf(((float)it / (delay / 2)) * PI);
		float64 tempxp = xp;

		xp = -sinf(((float)it / (float)(delay / 2)) * (float)PI) * boost;
		//lx = -((2 * (float)PI) / (float)delay) * cosf((2 * (float)PI * (float)it) / (float)delay) * boost;

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

		glutPostRedisplay(); //redraws window
		//glutTimerFunc(4, speedManager, 0);
		glutIdleFunc(speedManager);
	}
	else {

		glutPostRedisplay();
		//glutTimerFunc(4, speedManager, 0);
		glutIdleFunc(speedManager);
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
	float degree;
	float frequency;
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
	case 27: // ESC to exit fullscreen
		exit(0);
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
		break;
	case 45: //- will shrink bar
		if (barwidthIt > 0) {
			barwidthIt--;
			barwidth = barwidthArr[barwidthIt];
		}
		glutPostRedisplay();
		break;
	case 61: //= will enlarge bar
		if (barwidthIt < 2) {
			barwidthIt++;
			barwidth = barwidthArr[barwidthIt];
		}
		glutPostRedisplay();
		break;
	case 49: //1 will make stimulus single bar
		bars[0] = 0;
		bars[1] = 0;
		bars[2] = 0;
		bars[3] = 0;
		bars[4] = 0;
		glutPostRedisplay();
		break;
	case 50: //2 will make stimulus 5-bar grate
		bars[0] = -8;
		bars[1] = -4;
		bars[2] = 0;
		bars[3] = 4;
		bars[4] = 8;
		glutPostRedisplay();
		break;
	case 86: //V will request viewing angle
		printf("\nInput viewing angle in degrees: ");
		scanf("%f", &degree);
		barwidth = (float) 0.307975 * tanf(degree * PI / (2.0 * 180.0)) * 1342.281879;
		printf("%f", barwidth);
		glutPostRedisplay();
		break;
	case 118: //v will request viewing angle
		printf("\nInput viewing angle in degrees: ");
		scanf("%f", &degree);
		barwidth = (float) 0.307975 * tanf(degree * PI / (2.0 * 180.0)) * 1342.281879;
		printf("%f", barwidth);
		glutPostRedisplay();
		break;
	case 70: //F will request frequency
		printf("\nInput oscillation frequency in Hz: ");
		scanf("%f", &frequency);
		delay = 120.0 / frequency;
		break;
	case 102: //f will request frequency
		printf("\nInput oscillation frequency in Hz: ");
		scanf("%f", &frequency);
		delay = 120.0 / frequency;
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

	/*
	** This is where the closed-loop stuff ends
	*/
	//_putenv((char *) "__GL_SYNC_TO_VBLANK=1");
	/*
	PROC swp = wglGetProcAddress("wglSwapIntervalEXT");
	if (swp) {
		typedef BOOL(WINAPI *swp_t)(int);
		swp_t s = (swp_t)swp;
		s(0); // set it to 0 (turn off)
	}
	*/

	/*
	typedef void (APIENTRY * PFNWGLEXTSWAPCONTROLPROC) (int i);
	PFNWGLEXTSWAPCONTROLPROC wglSwapControl = NULL;
	wglSwapControl = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapControl != NULL) wglSwapControl(1);
	*/

	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	//glutInitWindowSize(1600, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Testing OpenGL");
	//SetWindowLong(hwnd, GWL_STYLE, 0);

	
	PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
	PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

	if (WGLExtensionSupported("WGL_EXT_swap_control"))
	{
		// Extension is supported, init pointers.
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		// this is another function from WGL_EXT_swap_control extension
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		wglSwapIntervalEXT(-1);
	}
	
	/*
	PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
	PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;
	// Extension is supported, init pointers.
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	// this is another function from WGL_EXT_swap_control extension
	wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	wglSwapIntervalEXT(1);
	*/
	//wglSwapIntervalEXT(1);

	fps_start = glutGet(GLUT_ELAPSED_TIME);
	glutFullScreen(); //This makes shit fullscreen
	//glutTimerFunc(1000, speedManager, 0);
	glutDisplayFunc(display);
	//printf("finished display\n");
	//glutTimerFunc(1, speedManager, 0);
	glutIdleFunc(speedManager);
	//printf("finished speedManager\n");

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0.0, 800, 0.0, 800);
	//gluOrtho2D(0.0, 1600, 0.0, 800);

	glutSpecialFunc(key_pressed);
	glutKeyboardFunc(letter_pressed);
	//printf("\n Will do something when the keys are hit");

	//printf("\n");
	//glutMainLoop();
	//return 0;

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
