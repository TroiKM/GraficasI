#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "figura.h"

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

Figura f;

void ejesCoordenada() {
	
	glLineWidth(1.5);
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.0);
	int i;
	glColor3f(0.1,0.1,0.1);
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


void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	
	aspectratio = (float) w / (float) h;

	if (w<=h) {
		
		glOrtho(-10.0,10.0,-10.0/aspectratio,10.0/aspectratio,1.0,50.0);

	}else {

		glOrtho(-10.0*aspectratio,10.0*aspectratio,-10.0,10.0,1.0,50.0);

	}


	glMatrixMode (GL_MODELVIEW);
}

void render(){
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		
	/* //Render Grid	
	
	glLineWidth(0.5);
	glPushMatrix();
    glColor3f( 0.8, 0.8, 0.8 );
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ ){
		xLocal = DEF_floorGridScale * loopX;
		glVertex3f( xLocal, -zExtent, 0.0 );
		glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ ){
		zLocal = DEF_floorGridScale * loopZ;
		glVertex3f( -xExtent, zLocal, 0.0 );
		glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();
    glPopMatrix();
	
	//Fin Grid*/

	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );

	//ejesCoordenada();
	f.dibujar();
	
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();

}

void comando(unsigned char key, int x, int y){
  switch(key)
    {
    case '1':
		f.setZonaSelec(1);
		render();
		break;
	case '2':
		f.setZonaSelec(2);
		render();
		break;
	case '3':
		f.setZonaSelec(3);
		render();
		break;
	case '4':
		f.setZonaSelec(4);
		render();
		break;
	case '5':
		f.setZonaSelec(5);
		render();
		break;
   
    case 'a':
    case 'A':
		if (f.getZonaSelec()==1){
			f.setAngCaderaIzq(f.getAngCaderaIzq() + 30.00);
		}else if(f.getZonaSelec()==2){
			f.setAngCaderaDer(f.getAngCaderaDer() + 30.00);
		}else if(f.getZonaSelec()==3){
			f.setAngHombroIzq(f.getAngHombroIzq() + 30.00);
		}else if(f.getZonaSelec()==4){
			f.setAngHombroDer(f.getAngHombroDer() + 30.00);
		}else if(f.getZonaSelec()==5){
			f.setAngCabeza(f.getAngCabeza() + 30.00);
		}
		render();
		break;

	case 'z':
    case 'Z':
		if (f.getZonaSelec()==1){
			f.setAngCaderaIzq(f.getAngCaderaIzq() - 30.00);
		}else if(f.getZonaSelec()==2){
			f.setAngCaderaDer(f.getAngCaderaDer() - 30.00);
		}else if(f.getZonaSelec()==3){
			f.setAngHombroIzq(f.getAngHombroIzq() - 30.00);
		}else if(f.getZonaSelec()==4){
			f.setAngHombroDer(f.getAngHombroDer() - 30.00);
		}else if(f.getZonaSelec()==5){
			f.setAngCabeza(f.getAngCabeza() - 30.00);
		}
		render();
		break;

	case 's':
    case 'S':
		if (f.getZonaSelec()==1){
			f.setAngRodillaIzq(f.getAngRodillaIzq() + 30.00);
		}else if(f.getZonaSelec()==2){
			f.setAngRodillaDer(f.getAngRodillaDer() + 30.00);
		}else if(f.getZonaSelec()==3){
			f.setAngCodoIzq(f.getAngCodoIzq() + 30.00);
		}else if(f.getZonaSelec()==4){
			f.setAngCodoDer(f.getAngCodoDer() + 30.00);
		}
		render();
		break;

	case 'x':
    case 'X':
		if (f.getZonaSelec()==1){
			f.setAngRodillaIzq(f.getAngRodillaIzq() - 30.00);
		}else if(f.getZonaSelec()==2){
			f.setAngRodillaDer(f.getAngRodillaDer() - 30.00);
		}else if(f.getZonaSelec()==3){
			f.setAngCodoIzq(f.getAngCodoIzq() - 30.00);
		}else if(f.getZonaSelec()==4){
			f.setAngCodoDer(f.getAngCodoDer() - 30.00);
		}
		render();
		break;

	case 'd':
    case 'D':
		if (f.getZonaSelec()==1){
			f.setAngTobilloIzq(f.getAngTobilloIzq() + 30.00);
		}else if(f.getZonaSelec()==2){
			f.setAngTobilloDer(f.getAngTobilloDer() + 30.00);
		}else if(f.getZonaSelec()==3){
			f.setAngManoIzq(f.getAngManoIzq() + 30.00);
		}else if(f.getZonaSelec()==4){
			f.setAngManoDer(f.getAngManoDer() + 30.00);
		}
		render();
		break;
	
	case 'c':
    case 'C':
		if (f.getZonaSelec()==1){
			f.setAngTobilloIzq(f.getAngTobilloIzq() - 30.00);
		}else if(f.getZonaSelec()==2){
			f.setAngTobilloDer(f.getAngTobilloDer() - 30.00);
		}else if(f.getZonaSelec()==3){
			f.setAngManoIzq(f.getAngManoIzq() - 30.00);
		}else if(f.getZonaSelec()==4){
			f.setAngManoDer(f.getAngManoDer() - 30.00);
		}
		render();
		break;
    
    }
}


int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(800,800);

	glutCreateWindow("Graficas I 2013 - Proyecto 1 - Andrea Balbas");

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(comando); 
	
	glutMainLoop();
	return 0;

}