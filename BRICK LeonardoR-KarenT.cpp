#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

/* Variables globales para la barra */
float tamanoBarrita = 3.0;
float movBarrita = 0.0;

/* Variables globales para la pelota */
float pelota_x = 0.0;
float pelota_y = 8.0;
float pelota_r = 0.2;
int reboto = 0;

/**
* Variables que determinan si ganaste
* o se ha perdido el juego.
*/
bool win = false;
bool game_over = false;

// FPS 
int fps = 50;

/**
* Clase encargada de la construccion de las paredes.
* Incluye:
** Punto x, y donde se inicia a dibujar la pared.
** Alto y ancho que ocupara la pared.
** Un boleano que avisa si son verticales u horizontales. 
*/
class Pared {
	
public:
	
	float WIDTH;
	float x,y;
	float length;
	bool vertical;

	/* Constructor por defecto */
	Pared(){
		this->Pared::Pared(0.0,0.0,0.0,false);
	}

	Pared( float x , float y , float length, bool vertical ){
		this->Pared::Pared(x,y,length,vertical,true);
	}

	Pared( float x , float y , float length , bool vertical , bool dibujar ){
		this->x = x;
		this->y = y;
		this->length = length;
		this->vertical = vertical;
		this->WIDTH = 0.5;

		if ( dibujar ) {
			dibujarPared();
		}
	}
	
	void dibujarPared(){
		
		glPushMatrix();
		glTranslatef(x,y,0);

		int orientation;

		if ( vertical ){

			/* Evitamos division por cero */
			if ( x != 0 ){
				orientation = x / abs(x);
			} else {
				orientation = 1;
			}
		} else {

			/* Evitamos division por cero */
			if ( y != 0 ){
				orientation = y / abs(y);
			} else {
				orientation = 1;
			}

		}


		glColor3f( 0.0, 1.0, 0.0 );

		glBegin( GL_LINES );
			
			if ( vertical ){
				/*  Piso y techo */
				glVertex3f( 0.0 , 0.0 , 0.0 );
				glVertex3f( orientation*WIDTH , 0.0 , 0.0 );

				glVertex3f( 0.0 , length, 0.0 );
				glVertex3f( orientation*WIDTH , length , 0.0 );

				/* Paredes */ 
				glVertex3f( 0.0 , 0.0 , 0.0 );
				glVertex3f( 0.0 , length , 0.0 );

				glVertex3f( orientation*WIDTH , 0.0 , 0.0 );
				glVertex3f( orientation*WIDTH , length , 0.0 );
			} else {
				/* Piso y techo */
				glVertex3f( 0.0 , 0.0 , 0.0 );
				glVertex3f( 0.0 , orientation*WIDTH , 0.0 );

				glVertex3f( length , 0.0 , 0.0 );
				glVertex3f( length , orientation*WIDTH , 0.0 );

				/* Paredes */ 
				glVertex3f( 0.0 , 0.0 , 0.0 );
				glVertex3f( length , 0.0 , 0.0 );

				glVertex3f( 0.0 , orientation*WIDTH , 0.0 );
				glVertex3f( length , orientation*WIDTH , 0.0 );
			}
		glEnd();

		glPopMatrix();
	}

};

/* Arreglo contenedor de las 3 paredes a ser creadas */
Pared paredes[3];

/**
* Clase encargada de la construccion de los bricks.
*	 Hereda de la clase pared, conteniendo los mismos
*		atributos.
* Nuevos atributos:
** Un booleano que avisa si es un brick especoial. 
** Entero con la cantidad de vidas.
** Un booleano que avisa si el brick ha sido creado.
** Un booleano que avisa si el brick posee bonus.
*/
class Brick : public Pared {

public:
	bool strong;
	int lives;
	bool created;
	bool bonus;

	/* Clase constructura */
	Brick(){
		lives = 0;
		created = false;
	}

	Brick( float x , float y , float length , bool strong , bool bonus ){
		this->bonus = bonus;
		this->strong = strong;
		if ( strong ){
			lives = 1;
		} else {
			lives = 0;
		}
		created = true;
		this->Pared::Pared(x,y,length,false,false);
		dibujarBrick();
	}


	Brick( float x , float y , float length ){
		bonus = false;
		strong = false;
		lives = 0;
		created = true;
		this->Pared::Pared(x,y,length,false,false);
		dibujarBrick();
	}

	void dibujarBrick(){
		
		glPushMatrix();
		glTranslatef(x,y,0);

		int orientation;

		if ( vertical ){

			if ( x != 0 ){
				orientation = x / abs(x);
			} else {
				orientation = 1;
			}
		} else {

			if ( y != 0 ){
				orientation = y / abs(y);
			} else {
				orientation = 1;
			}

		}

		/**
		* Si el brick es especial se colorea de amarillo,
		* rojo en el caso contrario.
		*/
		if (strong){
			glColor3f( 1.0, 1.0, 0.0 );
		}
		else{
			glColor3f( 1.0, 0.0, 0.0 );
		}

		glBegin( GL_LINES );

			/**
			* Si los bricks son especiales y ya han sido
			*	golpeados una vez, su color cambia a cian
			*	y se simula una ruptura. Sino, son dibujados
			*	normalmente.
			*/
			if ( strong && lives == 0 ){

				glColor3f( 0.0, 1.0, 1.0 );
				
				/* Parte derecha del brick roto */
				glVertex3f( 0.0 , 0.0 , 0.0 );
				glVertex3f( (length/2)-0.1 , 0.0 , 0.0 );

				glVertex3f( length/2+0.1 , 0.0 , 0.0 );
				glVertex3f( length , 0.0 , 0.0 );

				glVertex3f( length/2+0.1 , 0.0 , 0.0 );
				glVertex3f( length/2+0.3 , WIDTH/3 , 0.0 );

				glVertex3f( length/2+0.3 , WIDTH/3 , 0.0 );
				glVertex3f( length/2-0.1 , (WIDTH/3)*2 , 0.0 );
				
				glVertex3f( length/2-0.1 , (WIDTH/3)*2 , 0.0 );
				glVertex3f( length/2+0.1 , WIDTH , 0.0 );

				/* Parte izquierda del brick roto */
				glVertex3f( 0.0 , orientation*WIDTH , 0.0 );
				glVertex3f( (length/2)-0.1 , orientation*WIDTH , 0.0 );

				glVertex3f( length/2+0.1 , orientation*WIDTH, 0.0 );
				glVertex3f( length , orientation*WIDTH , 0.0 );
				
				glVertex3f( length/2-0.1 , 0.0 , 0.0 );
				glVertex3f( length/2+0.1 , WIDTH/3 , 0.0 );

				glVertex3f( length/2+0.1 , WIDTH/3 , 0.0 );
				glVertex3f( length/2-0.3 , (WIDTH/3)*2 , 0.0 );

				glVertex3f( length/2-0.3 , (WIDTH/3)*2 , 0.0 );
				glVertex3f( length/2-0.1 , WIDTH , 0.0 );
			} else {
				/* Cara superior e inferior del brick no roto */
				glVertex3f( 0.0 , 0.0 , 0.0 );
				glVertex3f( length , 0.0 , 0.0 );

				glVertex3f( 0.0 , orientation*WIDTH , 0.0 );
				glVertex3f( length , orientation*WIDTH , 0.0 );
			}
			
			/* Caras laterales del brick */
			glVertex3f( 0.0 , 0.0 , 0.0 );
			glVertex3f( 0.0 , orientation*WIDTH , 0.0 );
			glVertex3f( length , 0.0 , 0.0 );
			glVertex3f( length , orientation*WIDTH , 0.0 );
		glEnd();

		glPopMatrix();
	}

};

/* Arrglo que contiene los 35 briks */
Brick bricks[5*7];
/* Arreglo con los indices de bricks especiales */
int indices_especiales[7];
/* Arreglo con los indices de bricks con bonus */
int indices_bonus[5];

/**
* Clase encargada de la construccion de la barra inferior.
* Incluye:
** Punto x, y donde se inicia a dibujar la barra.
** Alto y ancho que ocupara la barra.
*/
class Barrita {
	
public:
	float WIDTH;
	float x,y;
	float length;
	
	Barrita( float x , float y , float length ){
		this->x = x;
		this->y = y;
		this->length = length;
		this->WIDTH = 0.30;

		dibujarBarrita();
	}

	void dibujarBarrita(){
		
		x+=movBarrita;

		glPushMatrix();
		glTranslatef(x,y,0);
		glColor3f( 0.0, 0.0, 1.0 );

		glBegin( GL_LINES );
			/* Caras laterales */
			glVertex3f( 0.0 , 0.0 , 0.0 );
			glVertex3f( 0.0 , WIDTH , 0.0 );

			glVertex3f( length , 0.0 , 0.0 );
			glVertex3f( length , WIDTH , 0.0 );

			/* Caras inferior y superior */
			glVertex3f( 0.0 , 0.0 , 0.0 );
			glVertex3f( length , 0.0 , 0.0 );

			glVertex3f( 0.0 , WIDTH , 0.0 );
			glVertex3f( length , WIDTH , 0.0 );
		glEnd();

		glPopMatrix();
	}

};

/* Creamos una referencia a la barra */
Barrita *barrita;

/**
* Clase encargada de la construccion de los bonus.
* Incluye:
** Punto x, y, edge donde se inicia a dibujar el bonus.
** Un boleano que avisa si se encuentra activo o no. 
*/
class Bonus{

public:

	bool active;
	float x,y,edge;

	Bonus(){
		active = false;
	}

	Bonus( float x , float y ){
		active = true;
		this->x = x;
		this->y = y;
		this->edge = 0.4;
	}

	void dibujarBonus(){
		glPushMatrix();

		glColor3f( 1.0, 0.0, 1.0 );

		glBegin( GL_LINES );
			glVertex3f( x , y , 0.0 );
			glVertex3f( x+edge , y , 0.0 );
			
			glVertex3f( x , y+edge , 0.0 );
			glVertex3f( x+edge , y+edge , 0.0 );
			
			glVertex3f( x , y , 0.0 );
			glVertex3f( x , y+edge , 0.0 );
			
			glVertex3f( x+edge , y , 0.0 );
			glVertex3f( x+edge , y+edge , 0.0 );
		glEnd();

		y-=0.1;

	}

};


Bonus bonus[5*7];
int discovered_bonuses = 0;


/**
* Clase encargada de la 
* Incluye:
** Punto x, y donde de la ubicacion de la pelota.
** Valor v con la velocidad de la pelota.
*/
class Vector{
public:
	float x;
	float y;
	float v;

	Vector(){
		this->x = 0;
		this->y = 0;
		this->v = 0;
	}

	Vector(float x , float y , float v ){
		this->x = x;
		this->y = y;
		this->v = v;
	}
};

/**
* Al iniciar el juego, la pelota 
*	cae desde el centro.
*/
Vector pelota_vel(0.0,-1.0,0.1);

/**
* Clase encargada para gurdar valores especificos al moment
*	de calcular la colision con la pelota.
* Incluye:
** Punto x, y donde se inicia a dibujar el regtangulo.
** Alto y ancho que ocupara el rectangulo.
** Un boleano que avisa si son verticales u horizontales. 
*/
class Rect{

public:
	float x,y,width,length;
	bool vertical;

	Rect( float x , float y , float width , float length , bool vertical ){
		this->x = x;
		this->y = y;
		this->width = width;
		this->length = length;
		this->vertical = vertical;
	}
};

/**
* Clase encargada para gurdar valores especificos al momento
*	de calcular la colision con la pelota.
* Incluye:
** Punto x, y donde se encuentra el punto de interes.
*/
class Point{

public:
	float x,y;

	Point( float x , float y ){
		this->x = x;
		this->y = y;
	}
};


/**
* Funcion encargada para calcular la colision de la pelota
*	con las paredes.
*** Retorno
*	Dice por que lado esta chocando con la pared:
*		1 - Norte
*		2 - Este
*		3 - Sur
*		4 - Oeste
*		0 - No esta tocando
*/
int calcularChoque( Pared pa ){

	int orientation;

	if ( pa.vertical ){

		if ( pa.x != 0 ){
			orientation = pa.x / abs(pa.x);
		} else {
			orientation = 1;
		}
	} else {

		if ( pa.y != 0 ){
			orientation = pa.y / abs(pa.y);
		} else {
			orientation = 1;
		}

	}

	float width;
	float length;

	if ( pa.vertical ){
		length = pa.WIDTH*orientation;
		width = pa.length;
	}
	else{
		length = pa.length;
		width = pa.WIDTH*orientation;
	}

	/* Error */
	float error = 0.1;

	/* Norte y Sur*/
	bool bNorte = pelota_x >= pa.x && pelota_x <= pa.x+length && pelota_y+pelota_r+error >= pa.y && pelota_y+pelota_r+error <= pa.y+width && pelota_vel.y > 0;
	bool bSur = pelota_x >= pa.x && pelota_x <= pa.x+length && pelota_y-pelota_r-error >= pa.y && pelota_y-pelota_r-error <= pa.y+width && pelota_vel.y < 0;

	/* Este y Oeste */
	bool bEste = pelota_x+pelota_r+error >= pa.x && pelota_x+pelota_r+error <= pa.x+length && pelota_y >= pa.y && pelota_y <= pa.y+width && pelota_vel.x > 0;
	bool bOeste;

	if ( orientation < 0 ){
		bOeste = pelota_x-pelota_r-error >= pa.x+length && pelota_x-pelota_r-error <= pa.x && pelota_y >= pa.y && pelota_y <= pa.y+width && pelota_vel.x < 0;
	} else {
		bOeste = pelota_x-pelota_r-error >= pa.x && pelota_x-pelota_r-error <= pa.x+length && pelota_y >= pa.y && pelota_y <= pa.y+width && pelota_vel.x < 0;
	}
	
	/* Noreste y Noroeste */
	bool bNorEste = (pow(pa.x-pelota_x-error,2)+pow(pa.y-pelota_y-error,2) <= pow(pelota_r,2)) && pelota_vel.x > 0 && pelota_vel.y > 0;
	bool bNorOeste = (pow(pa.x+length-pelota_x,2)+pow(pa.y-pelota_y,2)-error*2 <= pow(pelota_r,2)) && pelota_vel.x < 0 && pelota_vel.y > 0;
	
	/* Sureste y Suroeste */
	bool bSurEste = (pow(pa.x-pelota_x,2)+pow(pa.y+width-pelota_y,2)-error*2 <= pow(pelota_r,2)) && pelota_vel.x > 0 && pelota_vel.y < 0;
	bool bSurOeste = (pow(pa.x+length-pelota_x,2)+pow(pa.y+width-pelota_y,2)-error*2 <= pow(pelota_r,2)) && pelota_vel.x < 0 && pelota_vel.y < 0;

	if ( bNorte ) {
		return 1;
	}
	
	if ( bEste || bNorEste || bSurEste ) {
		return 2;
	}

	if ( bSur ) {
		return 3;
	}

	if ( bOeste || bNorOeste || bSurOeste ) {
		return 4;
	}

	return 0;
}

/**
* Funcion encargada para calcular la colision de la pelota
*	con los bricks.
*** Retorno
*	Dice por que lado esta chocando con el brick:
*		1 - Norte
*		2 - Este
*		3 - Sur
*		4 - Oeste
*		0 - No esta tocando
*/
int calcularChoqueBrick( Brick b ){

	int ret = calcularChoque(b);
	
	return ret;
}

/**
* Funcion encargada para calcular la colision de la pelota
*	con la barra.
*** Retorno
*	Dice por que lado esta chocando con el brick:
*		1 - Norte
*		2 - Este
*		3 - Sur
*		4 - Oeste
*		0 - No esta tocando
*/
int calcularChoque( Barrita b ){

	Pared p(b.x,b.y,b.length,false,false);
	p.WIDTH = b.WIDTH;

	return calcularChoque(p);
}

/**
* Funcion encargada para saber si un bonus fue atrapado.
*
*** Retorno
*		Auncia si el bonus fue atrapado o no por la barra.
*/
bool bonusCapturado( Bonus b ){

	return ((b.x >= barrita->x && b.x <= barrita->x+barrita->length) || (b.x+b.edge >= barrita->x && b.x+b.edge <= barrita->x+barrita->length) ) && b.y >= barrita->y && b.y <= barrita->y+barrita->WIDTH;

}

/**
* Funcion encargada de devolver el vector de 
*	rebote de la pelota en caso de coincidir con
*	una pared.
*/
Vector normalPared( Pared pared , int lado_choque ){
	
	switch( lado_choque ){

	case 1:
		return Vector(0.0,1.0,1.0);
		break;
	case 2:
		return Vector(1.0,0.0,1.0);
		break;
	case 3:
		return Vector(0.0,-1.0,1.0);
		break;
	case 4:
		return Vector(-1.0,0.0,1.0);
		break;

	}

}

/**
* Funcion encargada de cambiar la direccion del vector 
*	de la pelota en caso de coincidir con
*	un brick.
*/
Vector cambiarDireccionPelota( Brick brick , int lado_choque ){

	switch( lado_choque ){
		/* Norte y sur */
		case 1:
		case 3:
			return Vector(pelota_vel.x,pelota_vel.y*-1,pelota_vel.v);
			break;
		/* Este y oeste*/
		case 2:
		case 4:
			return Vector(pelota_vel.x*-1,pelota_vel.y,pelota_vel.v);
			break;
	}

}

/**
* Funcion encargada de cambiar la direccion del vector 
*	de la pelota en caso de coincidir con
*	una pared.
*/
Vector cambiarDireccionPelota( Pared pared , int lado_choque ){

	Vector wallN = normalPared(pared,lado_choque);
	
	float pProd = wallN.x*pelota_vel.x+wallN.y*pelota_vel.y;

	float x_rand = (rand() % 50 + -25)*0.01;
	float y_rand = (rand() % 50 + -25)*0.01;

	if ( rand() % 2 == 0 ){
		x_rand*=-1;
	}
	
	if ( rand() % 2 == 0 ){
		y_rand*=-1;
	}

	return Vector( (pelota_vel.x-(2 * wallN.x)*pProd)+( lado_choque == 1 ? x_rand:0) , (pelota_vel.y-(2*wallN.y)*pProd),pelota_vel.v);
}

/**
* Funcion encargada de cambiar la direccion del vector 
*	de la pelota en caso de coincidir con
*	la barra.
*/
Vector cambiarDireccionPelota( Barrita b , int lado_choque ){

	Pared p(b.x,b.y,b.length,false,false);
	p.WIDTH = b.WIDTH;

	Vector wallN = normalPared(p,lado_choque);
	
	float pProd = wallN.x*pelota_vel.x+wallN.y*pelota_vel.y;

	float x_rand = (rand() % 50 + -25)*0.01;
	float y_rand = (rand() % 50 + -25)*0.01;

	if ( rand() % 2 == 0 ){
		x_rand*=-1;
	}
	
	if ( rand() % 2 == 0 ){
		y_rand*=-1;
	}

	return Vector( (pelota_vel.x-(2 * wallN.x)*pProd)+x_rand , (pelota_vel.y-(2*wallN.y)*pProd),pelota_vel.v);
}

/**
* Funcion encargada para dibujar los circulos de las
*	explosiones.
*/
void dibujarCirculo( float x , float y , float radio , float red , float green , float blue ){
	
	float DEG_TO_RADIAN = 0.0174532925;
	
	glPushMatrix();

	glTranslatef(x,y,0.0);
	glColor3f( red , green , blue );

	glBegin(GL_LINE_LOOP);
		for (int i=0; i < 360; i++){
			float degInRad = i*DEG_TO_RADIAN;
			glVertex2f(cos(degInRad)*radio,sin(degInRad)*radio);
		}
	glEnd();

	glPopMatrix();
}

/**
* Clase encargada de generar la explosion del 
*	brick especial.
** Incluye:
* *  El punto x, y centro de la explosion.
* * La cantidad de particulas a ser dispersadas.
* * Radio de la explosion y las particulas.
*/
class Explosion{

public:

	float x,y,t;

	float particulas[10][3];
	float radio_particulas;

	float exp_radio;
	bool activo;

	/* Se empieza sin ninguna explosion. */
	Explosion(){
		activo = false;
	}

	/* Al momento de ser activada una explosión */
	Explosion( float x , float y ){

		activo = true;
		this->x = x;
		this->y = y;
		this->t = 0.0;
		exp_radio = 0.3;
		radio_particulas = 0.1;

		float DEG_TO_RADIAN = 0.0174532925;
		
		srand(time(NULL));

		for ( int i = 0 ; i < 10 ; i++ ){

			int point = rand() % 360;
			float degInRad = point*DEG_TO_RADIAN;

			particulas[i][0] = x+(cos(degInRad)*exp_radio);
			particulas[i][1] = y+(sin(degInRad)*exp_radio);
			particulas[i][2] = 0.1;
		}

		dibujarExplosion();
	}

	void dibujarExplosion(){

		for ( int i = 0 ; i < 10 ; ++i ){
			/* Utilizando la funcion parametrica de la recta. */
			float vector_dir_x = x*(1-t)+particulas[i][0]*t;
			float vector_dir_y = y*(1-t)+particulas[i][1]*t;

			dibujarCirculo(vector_dir_x,vector_dir_y,particulas[i][2],0.0,1.0,1.0);
		}
		
		t+=0.1;

		if ( t >= 10 ){
			activo = false;
		}
	}

	void dibujarExplosion( float r , float g , float b ){

		for ( int i = 0 ; i < 10 ; ++i ){
			
			float vector_dir_x = x*(1-t)+particulas[i][0]*t;
			float vector_dir_y = y*(1-t)+particulas[i][1]*t;

			dibujarCirculo(vector_dir_x,vector_dir_y,particulas[i][2],r,g,b);
		}
		
		t+=0.1;

		if ( t >= 10 ){
			activo = false;
		}
	}

};

/* Creamos el arreglo de posibles explosiones */
Explosion explosiones[7];
/*
* Variable que controla la cantidad de 
*	 explosiones ocurridas 
*/
int num_explosiones = 0;
/**
* Creamos un arreglo de explosiones que simulan
*	fuegos artificiales y solo aparecen al momento
*	de ganar el juego.
*/
Explosion fuegos[2];


class Pelota{
	
public:
	float r;
	
	Pelota( float r ){
		this->r = r;
		dibujarPelota();
	}

	void dibujarPelota(){

		glPushMatrix();

		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_LINE_SMOOTH );
		
		float DEG_TO_RADIAN = 0.0174532925;

		float new_x = pelota_x+(pelota_vel.x*pelota_vel.v);
		float new_y = pelota_y+(pelota_vel.y*pelota_vel.v);

		if ( new_y < 0 ){
			game_over = true;
		}

		if ( reboto ){
			reboto--;
		}

		float norte = new_y+r;
		float sur = new_y-r;
		float este = new_x+r;
		float oeste = new_x-r;
		
		if ( int lado_hit = calcularChoque(*barrita) ){
			pelota_vel = cambiarDireccionPelota(*barrita,lado_hit);
			reboto = 2;
		}

		int paredes_length = 3;
		int bricks_length = 5*7;
		int lado_choque;

		if ( !reboto ){
			
			for ( int i = 0 ; i < paredes_length ; ++i ){
			
				if ( lado_choque = calcularChoque(paredes[i]) ){
					pelota_vel = cambiarDireccionPelota( paredes[i] , lado_choque );
					reboto = 2;
				}
			}

			if ( !reboto ){
				for ( int i = 0 ; i < bricks_length ; ++i ){

					if ( bricks[i].lives > -1 ){
						if ( lado_choque = calcularChoqueBrick(bricks[i]) ){
							bricks[i].lives--;
							pelota_vel = cambiarDireccionPelota( bricks[i] , lado_choque );
							reboto = 2;

							
							if ( bricks[i].strong && bricks[i].lives == -1 && num_explosiones < 7 ){
								explosiones[num_explosiones] = Explosion(bricks[i].x+bricks[i].length/2,bricks[i].y+bricks[i].WIDTH/2);
								num_explosiones++;
							}

							if ( bricks[i].bonus && bricks[i].lives == -1 ){
								bonus[discovered_bonuses] = Bonus(bricks[i].x,bricks[i].y);
								bonus[discovered_bonuses].x+= bricks[i].length/2 - bonus[discovered_bonuses].edge/2;
								bonus[discovered_bonuses].x+= bricks[i].WIDTH/2 - bonus[discovered_bonuses].edge/2;
								bonus[discovered_bonuses].dibujarBonus();
								discovered_bonuses++;
							}
						}
					}
				}
			}
		}
		glTranslatef(new_x,new_y,0);

		pelota_x = new_x;
		pelota_y = new_y;

		glColor3f( 1.0, 1.0, 1.0 );

		glBegin(GL_LINE_LOOP);
			for (int i=0; i < 360; i++){
				float degInRad = i*DEG_TO_RADIAN;
				glVertex2f(cos(degInRad)*r,sin(degInRad)*r);
			}
		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);

		glPopMatrix();
	}

};

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0) {
		h=1;
	}

	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	
	aspectratio = (float) w / (float) h;

	if (w<=h) {
		glOrtho(-10.0,10.0,-10.0/aspectratio,10.0/aspectratio,1.0,50.0);
	} else {
		glOrtho(-10.0*aspectratio,10.0*aspectratio,-10.0,10.0,1.0,50.0);
	}

	glMatrixMode (GL_MODELVIEW);
}

/**
* Funcion que se encarga de dibujar
*	todo el programa principal.
*
*/
void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity ();                       
	gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glTranslatef(0.0,-9.0,0.0);

	/* Se dibujan las paredes */
	Pared paredIzq (-8.0,0.0,18.0,true);
	Pared paredDer (8.0,0.0,18.0,true);
	Pared paredTop (-8.0,17.5,16.0,false);

	paredes[0] = paredIzq;
	paredes[1] = paredDer;
	paredes[2] = paredTop;

	/* Empezamos con los bricks */

	float ini_x = -6.75;
	float ini_y = 15.0;
	float brick_length = 1.2;

	int strongs_count = 0;
	int bonus_count = 0;
	srand(time(NULL));

	/* Si aun no pierdes el juego */
	if ( !game_over ){
		
		bool todos_muertos = true;

		for ( int i = 0 ; i < 7*5 ; ++i ){
			
			if ( bricks[i].lives > -1 ) {

				todos_muertos = false;

				int offset_x = (i % 7)*2;
				int offset_y = (i / 7);
			
				if (!bricks[i].created) {
					/* Para la primera vez creamos los bricks */
					bool strong = false;
					for ( int j = 0 ; j < 7 ; ++j ){
						if ( indices_especiales[j] == i ){
							strong = true;
							break;
						}
					}

					bool bonus = false;
					for ( int j = 0 ; j < 5 ; ++j ){
						if ( indices_bonus[j] == i ){
							bonus = true;
							break;
						}
					}

					bricks[i] = Brick( ini_x+offset_x , ini_y-offset_y*1.3 , brick_length , strong , bonus );
				} else {
					/**
					* Aun le quedan vidas al brick.
					*/
					bricks[i].x = ini_x+offset_x;
					bricks[i].y = ini_y-offset_y*1.3;
					bricks[i].dibujarBrick();
				}
			}
		}

		if ( todos_muertos ){
			win = true;
		}
	} else {
		/* Si termino el juego y no ganaste */
		if (!win){
			glPushMatrix();

			glScalef(0.01, 0.01, 0.01);
			glTranslatef(-170.0,1000.0,0.0);
			glLineWidth(3.0);
			glColor3f(0.0,1.0,0.0);

			/* Se comienza la impresion del mensaje GAME OVER */
			char frase[10] = "GAME\nOVER";

			for ( int i = 0 ; i < 9 ; ++i ){

				if ( frase[i] == '\n' ){
					glTranslatef(-420.0,-140.0,0.0);
				}
				else{
					glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, frase[i]);
				}
			}
		
			glLineWidth(1.0);
			glPopMatrix();
			
			/* Se dibujan todos los bricks como al incio 
			*	del programa y caen todos juntos hasta 
			*	el final dejando solo el mensaje de 
			*	GAME OVER.
			*/
			for ( int i = 0 ; i < 5*7 ; ++i ){

				bricks[i].y-=0.1;
			
				if ( bricks[i].y > -1.0 ){
					bricks[i].lives = 1;
					bricks[i].dibujarBrick();
				}
			}
		}
	}
	
	/* El momento en el que el bonus es liberado. */
	for ( int i = 0 ; i < 7*5 ; ++i ) {
		if ( bonus[i].active ){
			bonus[i].dibujarBonus();

			if ( bonus[i].y < -1.0 ){
				bonus[i].active = false;
			} else {
				if ( bonusCapturado(bonus[i]) ){
					
					srand(time(NULL));
					int coin = rand() % 2;

					if ( coin ){
						barrita->length+=0.5;
					} else {
					 	pelota_vel.v+=0.05;
					}
					bonus[i].active = false;
				}
			}
		}
	}

	/* Dibujamos la barrita con cada movimiento */
	if ( barrita == 0 ){
		barrita = new Barrita(-(tamanoBarrita/2),0.0,tamanoBarrita);
	} else {
		barrita->dibujarBarrita();
	}

	/* Para dibujar las explosiones */
	for ( int i = 0 ; i < 7 ; ++i ){
		if ( explosiones[i].activo ){
			explosiones[i].dibujarExplosion();
		}
	}
	
	Pelota pelota(pelota_r);

	/* Al momento de ganar el juego */
	if ( win ){
		glPushMatrix();

		glScalef(0.01, 0.01, 0.01);
		glTranslatef(-130.0,1000.0,0.0);
		glLineWidth(4.0);
		glColor3f(0.0,1.0,1.0);

		/* Impresion del mensaje YOU WIN */
		char frase[8] = "YOU\nWIN";

		for ( int i = 0 ; i < 8 ; ++i ){

			if ( frase[i] == '\n' ){
				glTranslatef(-310.0,-140.0,0.0);
			}
			else{
				
				glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, frase[i]);
			}
		}
		
		glLineWidth(1.0);
		glPopMatrix();

		/* Explosion de fuegos artificales */
		if ( !fuegos[0].activo ){
			fuegos[0] = Explosion(-6.0,11.0);
		} else {
			fuegos[0].dibujarExplosion(1.0,0.0,1.0);
		}

		if ( !fuegos[1].activo ){
			fuegos[1] = Explosion(6.0,11.0);
		} else {
			fuegos[1].dibujarExplosion(0.0,0.6,1.0);
		}

	}

	glutSwapBuffers();
}

/**
* Funcion que mueve la barra a travez
* de entrada estandar.
*/
void teclas(unsigned char key, int x, int y){

	switch ( key ){
	/* Mueve la barra hacia el lado izquierdo */
	case 'z':
	case 'Z':
		if ( barrita->x > -8 ){
			barrita->x-=0.5;
		}
		break;
	/* Mueve la barra hacia el lado derecho */
	case 'x':
	case 'X':
		if ( barrita->x < 8-barrita->length ){
			barrita->x+=0.5;
		}
		break;
	}

}

void timerFunction( int value ){	
	render();
	glutTimerFunc(1000/fps,timerFunction,value);
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(600,600);

	glutCreateWindow("Proyecto II | Bricks | 08-10921 && 09-10855");

	/* Verificamos de entre los indices */
	int indices[7*5];

	for ( int i = 0 ; i < 7*5 ; ++i ){
		indices[i] = i;
	}
	
	/* Reordenamos el arreglo de forma aleatoria */
	srand(time(NULL));
	std::random_shuffle(indices,indices+5*7);

	/**
	* Bricks especiales coinciden con los primeros
	*	7 digitos arrojados por el random_shuffle
	*/
	for ( int i = 0 ; i < 7 ; ++i ){
		indices_especiales[i] = indices[i];
	}

	srand(time(NULL));
	std::random_shuffle(indices,indices+5*7);
	
	/**
	* Bricks con bonus que coinciden con los primeros
	*	5 digitos arrojados por el random_shuffle
	*/
	for ( int i = 0 ; i < 5 ; ++i ){
		indices_bonus[i] = indices[i];
	}

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	
	glutKeyboardFunc(teclas);
	glutTimerFunc(1000/fps,timerFunction,0);
	
	glutMainLoop();
	return 0;

}