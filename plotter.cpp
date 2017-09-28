#include <string.h>
#include <GL/glut.h>
#include<math.h>
#include<stdio.h>

const int segments = 100000;
const float segmentlen = 1.0/segments;
const float screenxstart = -5.0f,screenxstop = 5.0f;
//Screen ranges from -5 to +5 on OpenGl coordinates
int funcdata[20]={0},degree;
float start,stop; //Range of x to be plotted
float funcval(float x){
    int i;
    float val;
    for(i=0;i<=degree;i++){
        val += funcdata[i]*pow(x,i);
    }
    return val;
}
void inpfunc(){
    int i;
    printf("Enter degree of polynomial.\n");
    scanf("%d",&degree);
    for(i=degree;i>0;i--){
        printf("Enter coefficient of x^%d.: ",i);
        scanf("%d",&funcdata[i]);
    }
    printf("Enter value of constant term.: ");
    scanf("%d",&funcdata[0]);
    printf("Enter range of x in form [start] [stop]: ");
    scanf("%f %f",&start,&stop);
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
	}
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double) w / (double) h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -7.0f);

	glBegin(GL_LINES);
		glVertex3f(-6.0f, 0.0f, 0.0f);
		glVertex3f(6.0f, 0.0f, 0.0f);
	glEnd();

    glPushMatrix();
    float neworix = (0.0f - start)*10/(stop-start)-5.0f; //transforms the position of y axis on screen depending on input range of function.
    glTranslatef(neworix, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, -2.75f, 0.0f);
		glVertex3f(0.0f, 2.75f, 0.0f);
	glEnd();
	glPopMatrix();
	glBegin(GL_LINE_STRIP);
        float i;
        for(i=0;i<1;i+=segmentlen){
            float x = start + i*(stop-start); //Actual value of x of function.
            float xdisp = screenxstart + i*(screenxstop-screenxstart); //Corresponding value of x on screen.
            glVertex3f(xdisp,funcval(x),0.0f);
        }
    glEnd();

	glutSwapBuffers();
}

void update(int value) {
	/*
	ang += 1;
	if (ang > 360.0f) {
		ang = 0.0f;
	}
	*/
	glutPostRedisplay();
	glutTimerFunc(166, update, 0);
}

int main(int argc, char** argv) {
    inpfunc();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 200);
	initRendering();
	glutCreateWindow("Graph Plotter");
	glutFullScreen();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop();
	return 0;
}
