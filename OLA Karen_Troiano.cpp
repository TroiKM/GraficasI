// Cubica

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0
#define PI 3.14159265

GLUnurbsObj *theNurb;
int ola = 0;
float L1, A1, S1, L2, A2, S2;
float  D1[2], D2[2];
 
float time = 1.0;

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

float knots[25];

void inicializarNots(){
	int i;
	float p = 1.0/18.0;
	for(i = 0; i < 25 ; i++) {
		if (i < 4) {
			knots[i] = 0.0;
		} else if (i > 20) {
			knots[i] = 1.0;
		} else {
			knots[i] = p;
			p = p + 1.0/18.0;
		}
	}
}

GLfloat puntosControl[21][21][3];

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0){
		h=1;
	}
	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);

}

float normalizarX(float x, float y){
	float n = sqrt(x*x+y*y);
	float magic = x/n;
	return magic;
}

float normalizarY(float x, float y){
	float n = sqrt(x*x+y*y);
	float magic = y/n;
	return magic;
}

void init_surface() {
	int f,c;
	int z = 10;
	for (f = 0; f < 21 ; f++) {
		int x = 10;
		for (c = 0; c < 21; c++) {
			puntosControl[f][c][0] = x;
			puntosControl[f][c][1] = 0.0;
			puntosControl[f][c][2] = z;
			x--;
		}
		z--;
	}
}

void oleaje(float t){
	int f,c;
	float y1,y2;
	float D1x,D1y,D2x,D2y;
	float productoPunto1, productoPunto2,w1,w2;
	int z = 10;
	for (f = 0; f < 21 ; f++) {
		int x = 10;
		for (c = 0; c < 21; c++) {

			D1x = normalizarX(D1[0],D1[1]);
			D1y = normalizarY(D1[0],D1[1]);

			productoPunto1 = (D1x*puntosControl[f][c][0] + D1y*puntosControl[f][c][2]);
			w1 = (2*PI)/L1;

			D2x = normalizarX(D2[0],D2[1]);
			D2y = normalizarY(D2[0],D2[1]);

			productoPunto2 = (D2x*puntosControl[f][c][0] + D2y*puntosControl[f][c][2]);
			w2 = (2*PI)/L2;

			y1 = A1 * sin(productoPunto1*w1 + (t*w1*S1));
			y2 = A2 * sin(productoPunto2*w2 + (t*w2*S2));

			puntosControl[f][c][1] = y1 + y2;
		}
	}
}

void init(){

	glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);

   init_surface();

   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 15.0);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

}

void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	

	// Luz y material

	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   


	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	
	glPushMatrix();

	gluBeginSurface(theNurb);
    
	gluNurbsSurface(theNurb, 
                   25, knots, 25, knots,
                   21 * 3, 3, &puntosControl[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);


	gluEndSurface(theNurb);
	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

void animacion(int value) {
	oleaje(time);
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	time = time + 0.1;
}


void Keyboard(unsigned char key, int x, int y)
{
	
  switch (key)
  {
	case 'r':
	case 'R':
		glutTimerFunc(10,animacion,1);
		break;
	case '1':
		ola = 1;
		break;
	case '2':
		ola = 2;
		break;
	case 'a':
	case 'A':
		if (ola == 1){
			L1 = L1 - 0.1;
			if (L1 == 0.0) {
				L1 = -0.1;
			}
		} 
		if(ola == 2){
			L2 = L2 - 0.1;
			if (L2 == 0.0) {
				L2 = -0.1;
			}
		
		}
		break;
	case 'z':
	case 'Z':
		if (ola == 1){
			L1 = L1 + 0.1;
			if (L1 == 0.0) {
				L1 = 0.1;
			}
		} 
		if(ola == 2){
			L2 = L2 + 0.1;
			if (L1 == 0.0) {
				L2 = 0.1;
			}
		}
		break;
	case 's':
	case 'S':
		if (ola == 1){
			A1 = A1 - 0.1;
		} 
		if(ola == 2){
			A2 = A2 - 0.1;
		}
		break;
	case 'x':
	case 'X':
		if (ola == 1){
			A1 = A1 + 0.1;
		} 
		if(ola == 2){
			A2 = A2 + 0.1;
		}
		break;
	case 'd':
	case 'D':
		if (ola == 1){
			S1 = S1 - 0.1;
		} 
		if(ola == 2){
			S2 = S2 - 0.1;
		}
		break;
	case 'c':
	case 'C':
		if (ola == 1){
			S1 = S1 + 0.1;
		} 
		if(ola == 2){
			S2 = S2 + 0.1;
		}
		break;
	case 'f':
	case 'F':
		if (ola == 1){
			D1[0] = D1[0] - 0.1;
		} 
		if(ola == 2){
			D2[0] = D2[0] - 0.1;
		}
		break;
	case 'v':
	case 'V':
		if (ola == 1){
			D1[0] = D1[0] + 0.1;
		} 
		if(ola == 2){
			D2[0] = D2[0] + 0.1;
		}
		break;
	case 'g':
	case 'G':
		if (ola == 1){
			D1[1] = D1[1] - 0.1;
		} 
		if(ola == 2){
			D2[1] = D2[1] - 0.1;
		}
		break;
	case 'b':
	case 'B':
		if (ola == 1){
			D1[1] = D1[1] + 0.1;
		} 
		if(ola == 2){
			D2[1] = D2[1] + 0.1;
		}
		break;
	case 27:
		exit(0);
		break;
  }

  printf("Ola 1: L: %f, A: %f, S: %f, Dx:%f, Dy:%f.\n",L1,A1,S1,D1[0],D1[1]);
  printf("Ola 2: L: %f, A: %f, S: %f, Dx:%f, Dy:%f.\n\n",L2,A2,S2,D2[0],D2[1]);

}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Proyecto III | Karen Troiano | 09-10855");

	init ();
	inicializarNots();

	L1 = 8.0;
	A1 = 0.4;
	S1 = 2.0;
	D1[0] = 0.0;
	D1[1] = -1.0;

	L2 = 4.0;
	A2 = 0.0;
	S2 = 0.0;
	D2[0] = 1.0;
	D2[1] = 1.0;

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	
	glutMainLoop();
	return 0;

}
