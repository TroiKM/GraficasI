#include "Ogre\Ogre.h"

class Nave{

public:

	Ogre::SceneManager* _sceneManager;
	Ogre::Camera* camera;

	Ogre::SceneNode* nodoNave;
	Ogre::SceneNode* nodoAlaIzq;
	Ogre::SceneNode* nodoAlaDer;
	
	Ogre::SceneNode* nodoAlaIzqSup;
	Ogre::SceneNode* nodoAlaDerSup;
	
	Ogre::SceneNode* nodoAlaIzqInf;
	Ogre::SceneNode* nodoAlaDerInf;

	Ogre::ManualObject* nave;
	float ancho_ala;
	int apertura;
	bool disparando;
	Ogre::AnimationState* animState[4];
	
	float boundry;
	float boundry_z;
	float translate;
	float translate_z;
	int lastMove;


	Nave( Ogre::SceneManager* sm , Ogre::Camera* cam );
	Ogre::SceneNode* Nave::crearAlaNave( Ogre::String nombre , bool motorArriba , bool reverso = false );
	Ogre::ManualObject* Nave::crearCuerpo(Ogre::String name, Ogre::String matName);
	Ogre::ManualObject* Nave::crearAla(Ogre::String name, Ogre::String matName);
	void Nave::abrirAlas();
	void Nave::cerrarAlas();
	void Nave::moverDerecha();
	void Nave::moverIzquierda();
	bool Nave::moverAdelante();
	void Nave::moverAlInicio();
	void Nave::disparar(Ogre::SceneNode* nodoNave, bool MotorArriba);



};