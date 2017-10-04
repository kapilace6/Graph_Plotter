#include <string.h>
#include <GL/glut.h>
#include<math.h>
#include<stdio.h>

#define segments 100000
int w = 1366, h = 768;
int mouseX, mouseY;
const float segmentlen = 1.0/segments;
const float screenxstart = -5.0f,screenxstop = 5.0f;
const float screenystart = -2.75f,screenystop = 2.75f;
//Screen ranges from -5 to +5 on OpenGl coordinates
float funcdata[20]={0},y[segments]={0};
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

void mouseMotion(int x, int y){
	mouseX = x;
}

void precompute(){
	int i;
	float x = startx;
	for(i=0;i<segments;i++){
		y[i] = funcval(x);
		if(y[i]<starty){
			starty = y[i];
		}
		if(y[i]>stopy){
		    stopy = y[i];	
		}
		x += (stopx-startx)/segments;
	}
	
}
void drawPointLoc(){
	int i = ((float)mouseX*100000)/1366;
	float x = 0.0f,fx = 0.0f;
	x = screenxstart + 10.0f*(i)/100000;
	fx = screenystart + 5.5*(y[i]-starty)/(stopy-starty);
	//printf("%d %d %f\n",i,mouseX,x);
	glPushMatrix();
	glTranslatef(x,0.0f,0.0f);
	glBegin(GL_LINES);
		glColor3f(0.1f,0.6f,0.1f);
		glVertex3f(0.0f,2.75f,0.0f);
		glVertex3f(0.0f,-2.75f,0.0f);
	glEnd();
	glPointSize(6);
	glBegin(GL_POINTS);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,fx,0.0f);
	glEnd();
	glPopMatrix();
	
}
void drawArrowAxes(){
	int left=1,right=1,top=1,bottom = 1;
	
	
	
	float neworix = (0.0f - startx)*10/(stopx-startx)-5.0f; //transforms the position of y axis on screen depending on input range of function.
    if(neworix>=5.0f){
	    neworix = 5.0f;	
	    right = 0;
	}else if(neworix<=-5.0f){
		neworix = -5.0f;
		left = 0;
	}
	
	float neworiy = (0.0f - starty)*5.5/(stopy-starty)-2.75f; //transforms the position of x axis on screen depending on y values.
	
	if(neworiy>=2.75f){
	    neworiy = 2.75f;
	    top = 0;	
	}else if(neworiy<=-2.75f){
		neworiy = -2.75f;
		bottom = 0;
	}
	
	glPushMatrix();
	glTranslatef(neworix, 0.0f, 0.0f);
	
	glBegin(GL_LINES);
		glVertex3f(0.0f, -2.80f, 0.0f);
		glVertex3f(0.0f, 2.80f, 0.0f);
	glEnd();
	
	if(bottom){ //Render bottom arrow?
	glBegin(GL_TRIANGLES);
        glVertex3f(-0.04f,-2.70f,0.0f);
        glVertex3f(0.0f,-2.80f,0.0f);
        glVertex3f(0.04f,-2.70f,0.0f);
	glEnd();
	}
	
	if(top){ //Render top arrow?
	glBegin(GL_TRIANGLES);
        glVertex3f(-0.04f,2.70f,0.0f);
        glVertex3f(0.0f,2.80f,0.0f);
        glVertex3f(0.04f,2.70f,0.0f);
	glEnd();
	}
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f,neworiy,0.0f);
	glBegin(GL_LINES);
		glVertex3f(-5.1f, 0.0f, 0.0f);
		glVertex3f(5.1f, 0.0f, 0.0f);
	glEnd();
	
	if(left){ //Render left arrow?
	glBegin(GL_TRIANGLES);
        glVertex3f(-5.0,0.04,0.0f);
        glVertex3f(-5.1f,0.0f,0.0f);
        glVertex3f(-5.0f,-0.04f,0.0f);
	glEnd();
	}
	
	if(right){ //Render right arrow?
	glBegin(GL_TRIANGLES);
        glVertex3f(5.0,0.04,0.0f);
        glVertex3f(5.1f,0.0f,0.0f);
        glVertex3f(5.0f,-0.04f,0.0f);
	glEnd();
	}
	glPopMatrix();
}

void drawScene() {
	printf("Draw\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -7.0f);
	glColor3f(1.0f,1.0f,1.0f);
	
	drawArrowAxes();
	
	
	
	glBegin(GL_LINE_STRIP);
        int i;
        float x=startx; //Actual value of x of function.
        for(i=0;i<segments;i++){
            float xdisp = screenxstart + 10*(x-startx)/(stopx-startx); //Corresponding value of x on screen.
            float ydisp = screenystart + 5.5*(y[i]-starty)/(stopy-starty);
            glVertex3f(xdisp,ydisp,0.0f);
            x += (stopx-startx)/segments;
        }
    glEnd();
    
    drawPointLoc();

	glutSwapBuffers();
	glutPostRedisplay();
}

void update(int value) {
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
	
	glutPassiveMotionFunc(mouseMotion);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	
	
	glutMainLoop();
	
	return 0;
}
