#include <string.h>
#include <GL/glut.h>
#include<math.h>
#include<stdio.h>

#define segments 100000
const float segmentlen = 1.0/segments;
const float screenxstart = -5.0f,screenxstop = 5.0f;
const float screenystart = -2.75f,screenystop = 2.75f;
//Screen ranges from -5 to +5 on OpenGl coordinates
float funcdata[20]={0},preplot[segments]={0};
int degree;
float startx,stopx; //Range of x to be plotted
float starty=INFINITY,stopy=-INFINITY;
float funcval(float x){
    int i;
    float val=0;
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
        scanf("%f",&funcdata[i]);
    }
    printf("Enter value of constant term.: ");
    scanf("%f",&funcdata[0]);
    printf("Enter range of x in form [start] [stop] (0 0 for default): ");
    scanf("%f %f",&startx,&stopx);
    if(startx>=stopx){
        startx = -5;
        stopx = 5;
    }
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
void precompute(){
	int i;
	float x = startx;
	for(i=0;i<segments;i++){
		preplot[i] = funcval(x);
		//printf("%f ",funcval(x));
		if(preplot[i]<starty){
			starty = preplot[i];
		}
		if(preplot[i]>stopy){
		    stopy = preplot[i];	
		}
		x += (stopx-startx)/segments;
	}
	printf("ok");
	/*for(i=0;i<1000;i++){
		printf("%f ",preplot[i]);
	}*/
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -7.0f);

    glPushMatrix();
    float neworix = (0.0f - startx)*10/(stopx-startx)-5.0f; //transforms the position of y axis on screen depending on input range of function.
    if(neworix>5.0f){
	    neworix = 5.0f;	
	}else if(neworix<-5.0f){
		neworix = -5.0f;
	}
    glTranslatef(neworix, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, -2.80f, 0.0f);
		glVertex3f(0.0f, 2.80f, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
        glVertex3f(-0.1f,-2.75f,0.0f);
        glVertex3f(0.0f,-2.80f,0.0f);
        glVertex3f(0.1f,-2.75f,0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
        glVertex3f(-0.1f,2.75f,0.0f);
        glVertex3f(0.0f,2.80f,0.0f);
        glVertex3f(0.1f,2.75f,0.0f);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	float neworiy = (0.0f - starty)*5.5/(stopy-starty)-2.75f;
	if(neworix>2.75f){
	    neworix = 2.75f;	
	}else if(neworix<-2.75f){
		neworix = -2.75f;
	}
	glTranslatef(0.0f,neworiy,0.0f);
	glBegin(GL_LINES);
		glVertex3f(-5.1f, 0.0f, 0.0f);
		glVertex3f(5.1f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
        glVertex3f(-5.0,0.1,0.0f);
        glVertex3f(-5.1f,0.0f,0.0f);
        glVertex3f(-5.0f,-0.1f,0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
        glVertex3f(5.0,0.1,0.0f);
        glVertex3f(5.1f,0.0f,0.0f);
        glVertex3f(5.0f,-0.1f,0.0f);
	glEnd();
	glPopMatrix();
	glBegin(GL_LINE_STRIP);
        int i;
        float x=startx; //Actual value of x of function.
        for(i=0;i<segments;i++){
            float xdisp = screenxstart + 10*(x-startx)/(stopx-startx); //Corresponding value of x on screen.
            float ydisp = screenystart + 5.5*(preplot[i]-starty)/(stopy-starty);
            glVertex3f(xdisp,ydisp,0.0f);
            x += (stopx-startx)/segments;
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
    precompute();
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
