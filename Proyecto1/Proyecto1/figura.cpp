#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include "figura.h"

Figura::Figura(){
	zonaSelec = 0;
	angManoDer = 0.0;
	angManoIzq = 0.0;
	angCodoDer = 0.0;
	angCodoIzq = 0.0;
	angHombroDer = 0.0;
	angHombroIzq = 0.0;
	angCaderaDer = 0.0;
	angCaderaIzq = 0.0;
	angRodillaDer = 0.0;
	angRodillaIzq = 0.0;
	angTobilloDer = 0.0;
	angTobilloIzq = 0.0;
	angCabeza = 0.0;
}

void Figura::setZonaSelec(int zona){
	zonaSelec = zona;
}

int Figura::getZonaSelec(){
	return zonaSelec;
}

void Figura::setAngManoDer(float ang){
	angManoDer = ang;
}

int Figura::getAngManoDer(){
	return angManoDer;
}

void Figura::setAngManoIzq(float ang){
	angManoIzq = ang;
}

int Figura::getAngManoIzq(){
	return angManoIzq;
}

void Figura::setAngCodoDer(float ang){
	angCodoDer = ang;
}

int Figura::getAngCodoDer(){
	return angCodoDer;
}

void Figura::setAngCodoIzq(float ang){
	angCodoIzq = ang;
}

int Figura::getAngCodoIzq(){
	return angCodoIzq;
}

void Figura::setAngHombroDer(float ang){
	angHombroDer = ang;
}

int Figura::getAngHombroDer(){
	return angHombroDer;
}

void Figura::setAngHombroIzq(float ang){
	angHombroIzq = ang;
}

int Figura::getAngHombroIzq(){
	return angHombroIzq;
}

void Figura::setAngCaderaDer(float ang){
	angCaderaDer = ang;
}

int Figura::getAngCaderaDer(){
	return angCaderaDer;
}

void Figura::setAngCaderaIzq(float ang){
	angCaderaIzq = ang;
}

int Figura::getAngCaderaIzq(){
	return angCaderaIzq;
}

void Figura::setAngRodillaDer(float ang){
	angRodillaDer = ang;
}

int Figura::getAngRodillaDer(){
	return angRodillaDer;
}

void Figura::setAngRodillaIzq(float ang){
	angRodillaIzq = ang;;
}

int Figura::getAngRodillaIzq(){
	return angRodillaIzq;
}

void Figura::setAngTobilloDer(float ang){
	angTobilloDer = ang;
}

int Figura::getAngTobilloDer(){
	return angTobilloDer;
}

void Figura::setAngTobilloIzq(float ang){
	angTobilloIzq = ang;
}

int Figura::getAngTobilloIzq(){
	return angTobilloIzq;
}

void Figura::setAngCabeza(float ang){
	angCabeza = ang;
}

int Figura::getAngCabeza(){
	return angCabeza;
}

void Figura::dibujarTronco(){
	glPushMatrix();
	glColor3f(0.0,0.5,1.0);
	glBegin(GL_LINES);
		//tronco
		glVertex2f(0,3);
		glVertex2f(0,-2);
		//hombros
		glVertex2f(-1,2);
		glVertex2f(1,2);
		//cadera
		glVertex2f(-1,-2);
		glVertex2f(1,-2);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
	
}

void Figura::dibujarManoDer(){
	glPushMatrix();
	glTranslatef(2.0,0.0,0.0);
	glRotatef(angManoDer,0.0,0.0,1.0);
	if (zonaSelec == 4){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(1.0,0.64,0.0);
	}
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,1);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarAntebDer(){
	glPushMatrix();
	if (zonaSelec == 4){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.7,1.0,0.0);
	}
	glTranslatef(2.0,0.0,0.0);
	glRotatef(angCodoDer,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(2,0);
	glEnd();
	dibujarManoDer();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarBrazoDer(){
	glPushMatrix();
	if (zonaSelec == 4){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.5,0.0,1.0);
	}
	glTranslatef(1.0,2.0, 0.0);
	glRotatef(angHombroDer,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(2,0);
	glEnd();
	dibujarAntebDer();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarManoIzq(){
	glPushMatrix();
	glTranslatef(-2.0,0.0,0.0);
	glRotatef(angManoIzq,0.0,0.0,1.0);
	if (zonaSelec == 3){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(1.0,0.64,0.0);
	}
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,1);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarAntebIzq(){
	glPushMatrix();
	if (zonaSelec == 3){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.7,1.0,0.0);
	}
	glTranslatef(-2.0,0.0,0.0);
	glRotatef(angCodoIzq,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(-2,0);
	glEnd();
	dibujarManoIzq();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarBrazoIzq(){
	glPushMatrix();
	if (zonaSelec == 3){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.5,0.0,1.0);
	}
	glTranslatef(-1.0,2.0, 0.0);
	glRotatef(angHombroIzq,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(-2,0);
	glEnd();
	dibujarAntebIzq();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarPieDer(){
	glPushMatrix();
	if (zonaSelec == 2){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(1.0,0.64,0.0);
	}
	glTranslatef(0.0,-2.0,0.0);
	glRotatef(angTobilloDer,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(1,0);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarPantDer(){
	glPushMatrix();
	if (zonaSelec == 2){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.7,1.0,0.0);
	}
	glTranslatef(0.0,-2.0,0.0);
	glRotatef(angRodillaDer,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,-2);
	glEnd();
	dibujarPieDer();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarPiernaDer(){
	glPushMatrix();
	if (zonaSelec == 2){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.5,0.0,1.0);
	}
	glTranslatef(1.00,-2.0,0.0);
	glRotatef(angCaderaDer,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,-2);
	glEnd();
	dibujarPantDer();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarPieIzq(){
	glPushMatrix();
	if (zonaSelec == 1){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(1.0,0.64,0.0);
	}
	glTranslatef(0.0,-2.0,0.0);
	glRotatef(angTobilloIzq,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(-1,0);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarPantIzq(){
	glPushMatrix();
	if (zonaSelec == 1){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.7,1.0,0.0);
	}
	glTranslatef(0.0,-2.0,0.0);
	glRotatef(angRodillaIzq,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,-2);
	glEnd();
	dibujarPieIzq();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
	
}

void Figura::dibujarPiernaIzq(){
	glPushMatrix();
	if (zonaSelec == 1){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.5,0.0,1.0);
	}
	glTranslatef(-1.00,-2.0,0.0);
	glRotatef(angCaderaIzq,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,-2);
	glEnd();
	dibujarPantIzq();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujarCabeza(){
	glPushMatrix();
	glTranslatef(0.0,4.0,0.0);
	glRotatef(angCabeza,0.0,0.0,1.0);
	if (zonaSelec == 5){
		glColor3f(1.0,1.0,1.0);
	}else{
		glColor3f(0.0,0.5,1.0);
	}
	double radio,PI;
	radio = 1.0;
	PI =3.1415926535897932384626433832795;
	glBegin(GL_LINE_LOOP);
		for(double i = 0; i < 2 * PI; i += PI / 12)
 			glVertex3f(cos(i) * radio, sin(i) * radio, 0.0);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(0.0,-1.0,0.0);
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
}

void Figura::dibujar(){

	glLineWidth(2.0);
	glPointSize(8.0);
	
	dibujarTronco();
	dibujarBrazoDer();
	dibujarBrazoIzq();
	dibujarPiernaDer();
	dibujarPiernaIzq();
	dibujarCabeza();
	
}