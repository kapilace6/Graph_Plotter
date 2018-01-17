#include <string.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <vector>
#include <math.h>
#include <stdio.h>
#include "expression.cpp"
#define ratio 1.778645833
#define segments 100000

using namespace std;

int w, h;
int mouseX, mouseY;
int functionType;
float trigcoeff;

//Clipspace Coordinates
double segmentlen = 1.0 / segments;
double screenxstart = -5.15f, screenxstop = 5.15f;
double screenystart = -2.88f, screenystop = 2.88f;

//Function Variables
vector<double> funcdata;
GLdouble y[segments] = { 0 };
int degree;
double startx, stopx; // Range of x to be plotted
double starty = INFINITY, stopy = -INFINITY;

double scalingFactor;

//Text Display Variables
char expression[200];
parse p;
bool isZoom = false;

void dispString(double x, double y, char *string) {
	glRasterPos2f(x, y);
	for (int i = 0; string[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
}

//Computation Function
void precompute() {
	int i;
	double x = startx;
	starty = INFINITY;
	stopy = -INFINITY;

	for (i = 0; i < segments; i++) {
		y[i] = p.evalpost(x);
		if (y[i]<starty) {
			starty = y[i];
		}
		if (y[i]>stopy) {
			stopy = y[i];
		}
		x += (stopx - startx) / segments;
	}
}

//Input Function
void functionInput() {
	printf("\n\tEnter arithmetic expression : \n\t\t");
	fgets(expression, 200, stdin);

	printf("\n\n\tEnter range of x in form [start] [stop] (0 0 for default) : ");
	scanf("%lf %lf", &startx, &stopx);

	if (startx >= stopx) {
		startx = screenxstart;
		stopx = screenxstop;
	}
	p.intopost(expression);
}

//Inital Rendering Function
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

//Handle ASCII Keyboard Input
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
		case 'z':
			isZoom = true;
			break;
		case 's':
			isZoom = false;
			break;
		case '+':
			if ((startx + 10) <= (stopx - 10)) {
				startx += 10;
				stopx -= 10;
				precompute();
			}
			break;
		case '-':
			startx -= 10;
			stopx += 10;
			precompute();
			break;
	}
}

//Resizing Function
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void mouseMotion(int x, int y) {
	mouseX = x;
	mouseY = y;
}

//Handles Mouse Wheel Scrolling
void mouseScroll(int button, int dir, int x, int y) {
	if (isZoom) {
		double sx = startx;
		double ex = stopx;
		if (dir > 0) {
			startx += (ex - sx) / 4;
			stopx -= (ex - sx) / 4;
		}
		else {
			startx -= (ex - sx) / 2; //zoom out
			stopx += (ex - sx) / 2;
		}
		if ((stopx - startx) < 0.0001f) {
			startx = sx;
			stopx = ex;
		}
		if (startx > 1000000000 || startx < -1000000000 || stopx > 1000000000 || stopx < -1000000000) {
			startx = sx;
			stopx = ex;
		}
		precompute();
	}
}

//Handles Arrow Keyboard Input
void handleArrowpress(int key, int x, int y) {
	double sx = startx;
	double ex = stopx;
	switch (key) {
		case GLUT_KEY_UP:
			startx -= (ex - sx) / 2; //zoom out
			stopx += (ex - sx) / 2;
			break;
		case GLUT_KEY_DOWN: //zoom in
			startx += (ex - sx) / 4;
			stopx -= (ex - sx) / 4;
			break;
		case GLUT_KEY_LEFT:
			startx -= 10;
			stopx -= 10;
			break;
		case GLUT_KEY_RIGHT:
			startx += 10;
			stopx += 10;
			break;
	}

	if ((stopx - startx) < 0.0001f) {
		startx = sx;
		stopx = ex;
	}
	if (startx > 1000000000 || startx < -1000000000 || stopx > 1000000000 || stopx < -1000000000) {
		startx = sx;
		stopx = ex;
	}
	precompute();
}

//Select Mode Pointer
void drawPointLoc() {
	int i = ((float) mouseX * 100000) / w;
	double x = 0.0f, fx = 0.0f;

	x = screenxstart + (screenxstop - screenxstart) * i / 100000;
	fx = screenystart + (screenystop - screenystart) * (y[i] - starty) / (stopy - starty);
	double px = startx + (stopx - startx) * i / 100000;

	//Display Coordinates on Screen
	char Write[100];
	snprintf(Write, 100, "(%f, %f)", x, fx);
	dispString(-5.0, 2.5, Write);

	glPushMatrix();
		glTranslatef(x, 0.0f, 0.0f);
		glBegin(GL_LINES);
			glColor3f(0.1f, 0.6f, 0.1f);
			glVertex3f(0.0f, -2.90f, 0.0f);
			glVertex3f(0.0f, 2.90f, 0.0f);
		glEnd();

		glPointSize(6);

		glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, fx, 0.0f);
		glEnd();
	
	glPopMatrix();
}

//Draw Axes
void drawArrowAxes() {
	int left = 1, right = 1, top = 1, bottom = 1;

	// Transforms the Position of y-axis on screen depending on input range of function.
	double neworix = (0.0f - startx) * (screenxstop - screenxstart) / (stopx - startx) - screenxstop;
	if (neworix >= screenxstop) {
		neworix = screenxstop;
		right = 0;
	}
	else if (neworix <= screenxstart) {
		neworix = screenxstart;
		left = 0;
	}

	// Transforms the Position of x-axis on screen depending on y values.
	double neworiy = (0.0f - starty) * (screenystop - screenystart) / (stopy - starty) - screenystop;
	if (neworiy >= screenystop) {
		neworiy = screenystop;
		top = 0;
	}
	else if (neworiy <= screenystart) {
		neworiy = screenystart;
		bottom = 0;
	}

	glPushMatrix();
		glTranslatef(neworix, 0.0f, 0.0f);

		glBegin(GL_LINES);
			glVertex3f(0.0f, -2.90f, 0.0f);
			glVertex3f(0.0f, 2.90f, 0.0f);
		glEnd();

		glBegin(GL_TRIANGLES);

		// Render Bottom Arrow
		if (bottom) {
			glVertex3f(-0.04f, -2.78f, 0.0f);
			glVertex3f(0.0f, -2.88f, 0.0f);
			glVertex3f(0.04f, -2.78f, 0.0f);
		}
	
		//Render Top Arrow
		if (top) {
			glVertex3f(-0.04f, 2.78f, 0.0f);
			glVertex3f(0.0f, 2.88f, 0.0f);
			glVertex3f(0.04f, 2.78f, 0.0f);
		}
	
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, neworiy, 0.0f);
	
		glBegin(GL_LINES);
			glVertex3f(-5.153f, 0.0f, 0.0f);
			glVertex3f(5.153f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_TRIANGLES);

		//Render Left Arrow
		if (left) {
			glVertex3f(-5.0f, 0.04, 0.0f);
			glVertex3f(-5.153f, 0.0f, 0.0f);
			glVertex3f(-5.0f, -0.04f, 0.0f);
		}
	
		//Render Right Arrow
		if (right) {
			glVertex3f(5.0, 0.04, 0.0f);
			glVertex3f(5.153f, 0.0f, 0.0f);
			glVertex3f(5.0f, -0.04f, 0.0f);
		}
	
		glEnd();
	glPopMatrix();
}

//Main Drawing Scene
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -7.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	drawArrowAxes();

	char Write[100];
	if (isZoom)
		snprintf(Write, 100, "ZOOM MODE");
	else
		snprintf(Write, 100, "SELECT MODE");
	dispString(4.0, -2.65, Write);

	glBegin(GL_LINE_STRIP);	
	int i;
	double x = startx;
	for (i = 0; i < segments; i++) {
		double xdisp = screenxstart + (screenxstop - screenxstart) * (x - startx) / (stopx - startx);
		double ydisp = screenystart + (screenystop - screenystart)*(y[i] - starty) / (stopy - starty);
		glVertex3f(xdisp, ydisp, 0.0f);
		x += (stopx - startx) / segments;
	}
	glEnd();

	if (!isZoom)
		drawPointLoc();

	glutPostRedisplay();
	glutSwapBuffers();
}

void update(int value) {
	glutTimerFunc(166, update, 0);
}

//Main Function
int main(int argc, char** argv) {
	functionInput();
	try {
		precompute();
	}
	catch (...) {
		printf("\n\n\t\tInvalid.\n");
		return 0;
	}

	glutInit(&argc, argv);
	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w, h);

	initRendering();
	glutCreateWindow("Graph Plotter");
	glutFullScreen();

	glutPassiveMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseScroll);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleArrowpress);
	glutReshapeFunc(handleResize);

	glutMainLoop();

	return 0;
}
