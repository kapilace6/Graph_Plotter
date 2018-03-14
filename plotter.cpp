#include <string.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <vector>
#include <math.h>
#include <stdio.h>
#include "expression.cpp"

using namespace std;


#define segments 100000
int w = 1366, h = 768;
int mouseX, mouseY;
int functionType;
float trigcoeff;

const double segmentlen = 1.0 / segments;
const double screenxstart = -5.0f, screenxstop = 5.0f;
const double screenystart = -2.75f, screenystop = 2.75f;
//Screen ranges from -5 to +5 on OpenGl coordinates

vector<double> funcdata;
GLdouble y[segments] = { 0 };
int degree;
double startx, stopx; //Range of x to be plotted
double starty = INFINITY, stopy = -INFINITY;

double scalingFactor;

char expression[200];
parse p;

void dispString(double x, double y, char *string)
{
	glRasterPos2f(x, y);
	for (int i = 0; string[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
}

GLdouble polynomialFunc(float x) {
	int i;
	float val = 0;
	for (i = 0; i <= degree; i++) {
		val += funcdata[i] * pow(x, i);
	}
	return val;
}

GLdouble operation(double val) {
	switch (functionType) {
		case 1:
			return (GLdouble) sin(val * trigcoeff);
		case 2:
			return (GLdouble) cos(val * trigcoeff);
		case 3:
			return (GLdouble) tan(val * trigcoeff);
		case 4:
			return (GLdouble) 1.0 / sin(val * trigcoeff);
		case 5:
			return (GLdouble) 1.0 / cos(val * trigcoeff);
		case 6:
			return (GLdouble) 1.0 / tan(val * trigcoeff);
		case 9:
			return (GLdouble) polynomialFunc(val);
		default:
			return 0;
			break;
	}
}


void inpfunc() {
	int i;
	printf("Enter degree of polynomial.\n");
	scanf("%d", &degree);
	for (i = degree; i>0; i--) {
		printf("Enter coefficient of x^%d.: ", i);
		scanf("%lf", &funcdata[i]);
	}
	printf("Enter value of constant term.: ");
	scanf("%lf", &funcdata[0]);
	printf("Enter range of x in form [start] [stop] (0 0 for default): ");
	scanf("%lf %lf", &startx, &stopx);
	if (startx >= stopx) {
		startx = -5;
		stopx = 5;
	}
    p.intopost(expression);
}
void precompute() {
	int i;
	double x = startx;
    starty = INFINITY;
    stopy = -INFINITY;
	for (i = 0; i<segments; i++) {
		//y[i] = operation(x);
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

/*void functionInput() {
	printf("1 -> sin\n");
	printf("2 -> cos\n");
	printf("3 -> tan\n");
	printf("4 -> cosec\n");
	printf("5 -> sec\n");
	printf("6 -> cot\n");
	printf("9 -> polynomial function\n");
	while (1) {
		printf("Enter your choice: ");
		scanf("%d", &functionType);
		if (((functionType >= 1) && (functionType <= 6)) || (functionType == 9)) {
			break;
		}
		else {
			printf("Wrong choice!\n");
		}
	}
	if (functionType == 9) {
		printf("Enter degree of the polynomial: ");
		scanf("%d", &degree);
		double element;
		for (int s = 0; s <= degree; s++) {
			printf("Enter coefficient of x^%d term: ", s);
			scanf("%lf",  &element);
			funcdata.push_back(element);
		}
	}
	else {
		int angleunit;
		printf("1 -> degree\n");
		printf("2 -> radian\n");
		printf("Enter: ");
		scanf("%d", &angleunit);
		printf("Enter coefficient of x in your trig function: ");
		scanf("%f", &trigcoeff);
		if (angleunit == 1) {
			trigcoeff *= 0.01745329251; // pi / 180
		}
	}
	printf("Enter range of x in form [start] [stop] (0 0 for default): ");
	scanf("%f %f", &startx, &stopx);
	if (startx >= stopx) {
		startx = -5;
		stopx = 5;
	}
}*/

void functionInput(){
    printf("Enter arithmetic expression.\n");
    fgets(expression,200,stdin);
    printf("Enter range of x in form [start] [stop] (0 0 for default): ");
	scanf("%lf %lf", &startx, &stopx);
	if (startx >= stopx) {
		startx = -5;
		stopx = 5;
	}
    p.intopost(expression);
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);

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

void mouseScroll(int button, int dir, int x, int y){
    double sx = startx;
    double ex = stopx;
    if (dir > 0)
    {
        startx += (ex-sx)/4;
        stopx -= (ex-sx)/4;
    }
    else
    {
        startx -= (ex-sx)/2; //zoom out
		stopx += (ex-sx)/2;
    }
    precompute();
    
}

void handleArrowpress(int key, int x, int y) {
    double sx = startx;
    double ex = stopx;
	switch (key) {
	case GLUT_KEY_UP:
		startx -= (ex-sx)/2; //zoom out
		stopx += (ex-sx)/2;
		break;
	case GLUT_KEY_DOWN: //zoom in
        startx += (ex-sx)/4;
        stopx -= (ex-sx)/4;
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
    if((stopx-startx)<0.0001f){
        startx = sx;
        stopx = ex;
    }
    if(startx>1000000000||startx<-1000000000||stopx>1000000000||stopx<-1000000000){
        startx = sx;
        stopx = ex;
    }
    printf("%f\n",stopx-startx);
    precompute();
}

void drawPointLoc() {
	int i = ((float)mouseX * 100000) / w;
	double x = 0.0f, fx = 0.0f;

	x = screenxstart + 10.0f*(i) / 100000;
	fx = screenystart + 5.5*(y[i] - starty) / (stopy - starty);
    
    double px = startx + (stopx-startx)*i/100000;
	//printf("%d %d %f\n",i,mouseX,x);
    char Write[100];
    snprintf(Write,100,"(%f , %f)",px,y[i]);
    dispString(-4,2.5,Write);
    snprintf(Write,100,"(%f , %f)",starty,stopy);
    dispString(-4,1,Write);
    
	glPushMatrix();
	glTranslatef(x, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glColor3f(0.1f, 0.6f, 0.1f);
	glVertex3f(0.0f, 2.75f, 0.0f);
	glVertex3f(0.0f, -2.75f, 0.0f);
	glEnd();

	glPointSize(6);

	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, fx, 0.0f);
	glEnd();
	glPopMatrix();
}
void drawArrowAxes() {
	int left = 1, right = 1, top = 1, bottom = 1;

	double neworix = (0.0f - startx) * 10 / (stopx - startx) - 5.0f; //transforms the position of y axis on screen depending on input range of function.
	if (neworix >= 5.0f) {
		neworix = 5.0f;
		right = 0;
	}
	else if (neworix <= -5.0f) {
		neworix = -5.0f;
		left = 0;
	}

	double neworiy = (0.0f - starty)*5.5 / (stopy - starty) - 2.75f; //transforms the position of x axis on screen depending on y values.
	if (neworiy >= 2.75f) {
		neworiy = 2.75f;
		top = 0;
	}
	else if (neworiy <= -2.75f) {
		neworiy = -2.75f;
		bottom = 0;
	}

	glPushMatrix();
	glTranslatef(neworix, 0.0f, 0.0f);

	glBegin(GL_LINES);
	glVertex3f(0.0f, -2.80f, 0.0f);
	glVertex3f(0.0f, 2.80f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (bottom) { //Render bottom arrow?
		glVertex3f(-0.04f, -2.70f, 0.0f);
		glVertex3f(0.0f, -2.80f, 0.0f);
		glVertex3f(0.04f, -2.70f, 0.0f);
	}

	if (top) { //Render top arrow?
		glVertex3f(-0.04f, 2.70f, 0.0f);
		glVertex3f(0.0f, 2.80f, 0.0f);
		glVertex3f(0.04f, 2.70f, 0.0f);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, neworiy, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(-5.1f, 0.0f, 0.0f);
	glVertex3f(5.1f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	if (left) { //Render left arrow?
		glVertex3f(-5.0, 0.04, 0.0f);
		glVertex3f(-5.1f, 0.0f, 0.0f);
		glVertex3f(-5.0f, -0.04f, 0.0f);
	}

	if (right) { //Render right arrow?
		glVertex3f(5.0, 0.04, 0.0f);
		glVertex3f(5.1f, 0.0f, 0.0f);
		glVertex3f(5.0f, -0.04f, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

void drawScene() {
	//printf("Draw\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -7.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	drawArrowAxes();

	glBegin(GL_LINE_STRIP);
	int i;
	double x = startx; //Actual value of x of function.
	for (i = 0; i<segments; i++) {
		double xdisp = screenxstart + 10 * (x - startx) / (stopx - startx); //Corresponding value of x on screen.
		double ydisp = screenystart + 5.5*(y[i] - starty) / (stopy - starty);
		glVertex3f(xdisp, ydisp, 0.0f);
		x += (stopx - startx) / segments;
	}
	glEnd();

	drawPointLoc();

	glutPostRedisplay();

	glutSwapBuffers();
}

void update(int value) {

	glutTimerFunc(166, update, 0);
}

int main(int argc, char** argv) {
    
	functionInput();
    try{
        precompute();
    }catch(...){
        printf("Invalid.\n");
        return 0;
    }
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(200, 200);

	initRendering();
	glutCreateWindow("Graph Plotter");
	//glutFullScreen();

	glutPassiveMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseScroll);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleArrowpress);
	glutReshapeFunc(handleResize);

	glutMainLoop();

	return 0;
}
