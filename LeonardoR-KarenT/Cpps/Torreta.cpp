#include "Torreta.h"

Torreta::Torreta( Ogre::String nombre , Ogre::SceneManager* sm, float _x , float _y , float _z , float _rotacion ){

	x = _x;
	y = _y;
	z = _z;
	rotacion = _rotacion;
	_sceneManager = sm;

	nodoTorreta = _sceneManager->createSceneNode(nombre);
	Ogre::SceneNode* nodoToroInf = _sceneManager->createSceneNode(nombre+"_toroInf");
	Ogre::SceneNode* nodoToroSup = _sceneManager->createSceneNode(nombre+"_toroSup");
	Ogre::SceneNode* nodoArma1 = _sceneManager->createSceneNode(nombre+"_arma1");
	Ogre::SceneNode* nodoArma2 = _sceneManager->createSceneNode(nombre+"_arma2");

	Ogre::Entity* entTorreta = _sceneManager->createEntity("usb_cilindro02.mesh");
	Ogre::Entity* entToroInf = _sceneManager->createEntity("usb_torus.mesh");
	Ogre::Entity* entToroSup = _sceneManager->createEntity("usb_torus.mesh");
	Ogre::Entity* entArma1 = _sceneManager->createEntity("usb_cilindro.mesh");
	Ogre::Entity* entArma2 = _sceneManager->createEntity("usb_cilindro.mesh");

	entTorreta->setMaterialName("MatTorreta");
	entToroInf->setMaterialName("MatTorreta");
	entToroSup->setMaterialName("MatTorreta");
	entArma1->setMaterialName("MatTorreta");
	entArma2->setMaterialName("MatTorreta");

	nodoArma1->attachObject(entArma1);
	nodoArma2->attachObject(entArma2);

	nodoArma1->scale(0.5,2.5,0.5);

	nodoArma2->scale(1.5,-0.25,1.5);
	nodoArma2->translate(0.0,5.0,0.0);

	nodoArma1->addChild(nodoArma2);
	nodoArma1->scale(0.2,0.2,0.2);
	nodoArma1->translate(0.0,5.0,3.0);
	nodoArma1->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

	nodoTorreta->attachObject(entTorreta);
	nodoTorreta->scale(5.0,5.0,5.0);

	/* Toros */ 
	nodoToroInf->attachObject(entToroInf);
	nodoToroInf->translate(0.0,-6.0,0.0);
	nodoToroInf->scale(-0.5,1.0,-0.5);

	nodoToroSup->attachObject(entToroSup);
	nodoToroSup->translate(0.0,6.0,0.0);
	nodoToroSup->scale(-0.19,0.38,-0.19);

	nodoTorreta->addChild(nodoToroInf);
	nodoTorreta->addChild(nodoToroSup);
	nodoTorreta->addChild(nodoArma1);

	nodoTorreta->scale(0.4,0.4,0.4);
	nodoTorreta->translate(x,y,z);
	nodoTorreta->rotate(Ogre::Vector3(0.0,1.0,0.0),Ogre::Radian(Ogre::Degree(rotacion)));

	// Lase animado
	Ogre::SceneNode* nodeLaser = _sceneManager->createSceneNode("Laser_"+nombre);
	_sceneManager->getRootSceneNode()->addChild(nodeLaser);

	Ogre::Entity* laser = _sceneManager->createEntity("entLaser_"+nombre,"usb_laser.mesh");
	laser->setMaterialName("MatLaser");
	nodeLaser->attachObject(laser);
	nodeLaser->setScale(0.5f,0.5f,0.5f);
	nodeLaser->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

	float duration = 2.0;

	Ogre::Animation* animacion = _sceneManager->createAnimation("LaserAnim"+nombre,duration);
	animacion->setInterpolationMode(Ogre::Animation::IM_SPLINE);

	Ogre::NodeAnimationTrack* track = animacion->createNodeTrack(0,nodeLaser);
	
	float x_laser = rotacion > 0 ? 70.0 : -70.0;
	int y_laser = rotacion > 0 ? 1:-1;

	// Primer key de la animacion
	Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
	key->setTranslate(Ogre::Vector3(x,y+10.0,z));
	key->setScale(Ogre::Vector3(0.5,0.5,0.5));
	key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.25*y_laser,0.0)));
	//key->setRotation(Ogre::Quaternion(sqrt(0.5),sqrt(0.5),0.0,0.0));
	//key->setRotation(Ogre::Quaternion(0.97,0.0,0.0,0.27*z_laser));

	// Segundo key de la animacion
	key = track->createNodeKeyFrame(1.0f);
	key->setTranslate(Ogre::Vector3(x+x_laser,y,z+500.0));
	key->setScale(Ogre::Vector3(0.5,0.5,0.5));
	key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.25*y_laser,0.0)));
	//key->setRotation(Ogre::Quaternion(sqrt(0.5),sqrt(0.5),0.0,0.0));
	//key->setRotation(Ogre::Quaternion(0.97,0.0,0.0,0.27*z_laser));

	animState = _sceneManager->createAnimationState("LaserAnim"+nombre);
	animState->setEnabled(true);
	animState->setLoop(true);


}