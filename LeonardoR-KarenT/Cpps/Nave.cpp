#include "Nave.h"

Nave::Nave( Ogre::SceneManager* sm , Ogre::Camera* cam ){

	_sceneManager = sm;
	camera = cam;
	ancho_ala = -1.0;
	apertura = 0;
	boundry = 18.0;
	boundry_z = 1300.0;
	translate = 0.0;
	translate_z = 0.0;
	lastMove = 0;
	disparando = false;
	animState[0] = 0;
	animState[1] = 0;
	animState[2] = 0;
	animState[3] = 0;

	nodoNave = _sceneManager->createSceneNode("nave");
	nodoAlaIzq = _sceneManager->createSceneNode("AlaIzq");
	nodoAlaDer = _sceneManager->createSceneNode("AlaDer");

	Ogre::ManualObject* nave = crearCuerpo("nave", "MatPropio01");
	nodoNave->attachObject(nave);

	// Creando Alas izquierda
	nodoAlaIzqSup = crearAlaNave("alaIzqSup",true);
	nodoAlaIzqInf = crearAlaNave("alaIzqInf",false);
	nodoAlaIzqInf->translate(0.0,ancho_ala,0.0);
	// Rotando Alas izquierda
	//nodoAlaIzqSup->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(-15.0)));
	//nodoAlaIzqInf->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(15.0)));
	// Agregando alas izquierda
	nodoAlaIzq->addChild(nodoAlaIzqSup);
	nodoAlaIzq->addChild(nodoAlaIzqInf);
	nodoAlaIzq->translate(-5.0,2.0,-5.0);

	// Creando Alas derecha
	nodoAlaDerSup = crearAlaNave("alaDerSup",true,true);
	nodoAlaDerInf = crearAlaNave("alaDerInf",false,true);
	nodoAlaDerInf->translate(0.0,ancho_ala,0.0);
	// Rotando Alas derecha
	// Agregando alas derecha
	nodoAlaDer->addChild(nodoAlaDerSup);
	nodoAlaDer->addChild(nodoAlaDerInf);
	nodoAlaDer->translate(5.0,2.0+ancho_ala,-5.0);
	nodoAlaDer->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(180.0)));

	nodoNave->addChild(nodoAlaIzq);
	nodoNave->addChild(nodoAlaDer);
	nodoNave->setScale(0.3,0.3,0.3);
	nodoNave->setPosition(0.0,0.0,0.0);

}
		
Ogre::SceneNode* Nave::crearAlaNave( Ogre::String nombre , bool motorArriba , bool reverso ){

	Ogre::SceneNode* nodoAla = _sceneManager->createSceneNode("node_"+nombre);
	Ogre::SceneNode* nodoMotor0Ala = _sceneManager->createSceneNode(nombre+"_motor0");
	Ogre::SceneNode* nodoMotor1Ala = _sceneManager->createSceneNode(nombre+"_motor1");

	Ogre::ManualObject* ala = crearAla(nombre, "MatPropio01");
	Ogre::Entity* motor0 = _sceneManager->createEntity("usb_cilindro.mesh");
	Ogre::Entity* motor1 = _sceneManager->createEntity("usb_cilindro.mesh");

	float yTranslate = motorArriba ? 0.0 : 7.0;
	float yReverso = reverso ? 3.0 : 0.0;
	float yTranslateArma = motorArriba ? 0.0 : 4.0;

	nodoMotor0Ala->attachObject(motor0);
	nodoMotor0Ala->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));
	nodoMotor0Ala->translate(-6.0,3.0-yTranslate,-19.0);

	nodoMotor1Ala->attachObject(motor1);
	nodoMotor1Ala->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));
	nodoMotor1Ala->translate(-4.7,1.7-yTranslate+yReverso,-4.5);
	nodoMotor1Ala->scale(0.6,1.35,0.6);

	// Armas
	Ogre::SceneNode* nodoArma1 = _sceneManager->createSceneNode(nombre+"_arma1");
	Ogre::SceneNode* nodoArma2 = _sceneManager->createSceneNode(nombre+"_arma2");
	Ogre::Entity* entArma1 = _sceneManager->createEntity("usb_cilindro.mesh");
	Ogre::Entity* entArma2 = _sceneManager->createEntity("usb_cilindro.mesh");
	entArma1->setMaterialName("MatPropio01");
	entArma2->setMaterialName("MatPropio01");
	nodoArma1->attachObject(entArma1);
	nodoArma2->attachObject(entArma2);

	nodoArma1->scale(0.5,7.0,0.5);

	nodoArma2->scale(1.5,-0.25,1.5);
	nodoArma2->translate(0.0,5.0,0.0);

	nodoArma1->addChild(nodoArma2);
	nodoArma1->scale(0.7,0.7,0.7);
	nodoArma1->translate(-37.0,1.5-yTranslateArma,-42.0);
	nodoArma1->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

	nodoAla->addChild(nodoMotor0Ala);
	nodoAla->addChild(nodoMotor1Ala);
	nodoAla->addChild(nodoArma1);
	nodoAla->attachObject(ala);

	return nodoAla;
}

Ogre::ManualObject* Nave::crearCuerpo(Ogre::String name, Ogre::String matName) {
	
	float largo = -30.0;
	float escalaPunta = 4.0;
	float largoPunta = 4.0;

	Ogre::ManualObject* cube = new Ogre::ManualObject(name);
	cube->begin(matName);
	/* Cara mas cercana */
	cube->position(-5.0,10.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(0,0);
	cube->position(5.0,10.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(0,1);
	cube->position(-8.0,2.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(1,1);
	cube->position(8.0,2.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(1,0);
	cube->position(-5.0,-5.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(1,0);
	cube->position(5.0,-5.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(0,0);
	cube->position(0.0,-8.0,0.0);cube->normal(0.0,0.0,1.0);cube->textureCoord(0,1);
	
	/* Cara mas lejana*/
	cube->position(-5.0,10.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(5.0,10.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(-8.0,2.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(8.0,2.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(-5.0,-5.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(5.0,-5.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(0.0,-8.0,largo);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);

	/* Punta */
	cube->position(-5.0/escalaPunta, 10.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(5.0/escalaPunta, 10.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(-8.0/escalaPunta, 2.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(8.0/escalaPunta, 2.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(-5.0/escalaPunta, -5.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(5.0/escalaPunta, -5.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);
	cube->position(0.0, -8.0/escalaPunta, largo*largoPunta);cube->normal(0.0,0.0,-1.0);cube->textureCoord(1,0);

	/* Uniendo la cara frontal */
	cube->triangle(0,2,4);
	cube->triangle(1,5,3);
	cube->triangle(4,6,5);      
	cube->triangle(0,4,5);
	cube->triangle(0,5,1);

	/* Uniendo la cara lejana */ 
	cube->triangle(7,11,9);
	cube->triangle(8,10,12);
	cube->triangle(11,12,13);      
	cube->triangle(7,12,11);
	cube->triangle(7,8,12);

	/* Uniendo la punta */
	cube->triangle(14,18,16);
	cube->triangle(15,17,19);
	cube->triangle(18,19,20);      
	cube->triangle(14,19,18);
	cube->triangle(14,15,19);

	/* Uniendo caras */ 
	cube->triangle(0,8,7);
	cube->triangle(1,8,0);
	cube->triangle(3,8,1);      
	cube->triangle(3,10,8);
	cube->triangle(2,0,7);
	cube->triangle(2,7,9);
	cube->triangle(2,9,4);
	cube->triangle(4,9,11);      
	cube->triangle(5,10,3);
	cube->triangle(5,12,10);
	cube->triangle(4,11,6);
	cube->triangle(6,11,13);
	cube->triangle(5,6,12);      
	cube->triangle(6,13,12);

	/* Uniendo mariposas*/  
	cube->triangle(7,15,14);
	cube->triangle(8,15,7);
	cube->triangle(10,15,8);      
	cube->triangle(10,17,15);
	cube->triangle(9,7,14);
	cube->triangle(9,14,16);
	cube->triangle(9,16,11);
	cube->triangle(11,16,18);      
	cube->triangle(12,17,10);
	cube->triangle(12,19,17);
	cube->triangle(11,18,13);
	cube->triangle(13,18,20);
	cube->triangle(12,13,19);      
	cube->triangle(13,20,19);
	
	cube->end();
	return cube;
}

Ogre::ManualObject* Nave::crearAla(Ogre::String name, Ogre::String matName) {
	
	Ogre::ManualObject* cube = new Ogre::ManualObject(name);
	cube->begin(matName);
	
	/* Ala arriba */
	cube->position(0.0,0.0,0.0);cube->normal(0.0,-1.0,0.0);cube->textureCoord(0,0);
	cube->position(-45.0,0.0,-10.0);cube->normal(0.0,-1.0,0.0);cube->textureCoord(0,1);
	cube->position(-45.0,0.0,-25.0);cube->normal(0.0,-1.0,0.0);cube->textureCoord(1,1);
	cube->position(0.0,0.0,-25.0);cube->normal(0.0,-1.0,0.0);cube->textureCoord(1,0);

	/* Ala abajo */
	cube->position(0.0,ancho_ala,0.0);cube->normal(0.0,1.0,0.0);cube->textureCoord(0,0);
	cube->position(-45.0,ancho_ala,-10.0);cube->normal(0.0,1.0,0.0);cube->textureCoord(0,1);
	cube->position(-45.0,ancho_ala,-25.0);cube->normal(0.0,1.0,0.0);cube->textureCoord(1,1);
	cube->position(0.0,ancho_ala,-25.0);cube->normal(0.0,1.0,0.0);cube->textureCoord(1,0);
	
	/* Ala arriba */
	cube->triangle(2,1,0);
	cube->triangle(2,0,3);
	/* Ala abajo */ 
	cube->triangle(6,4,5);
	cube->triangle(6,7,4);

	/* Uniendo Ala */
	cube->triangle(0,1,5);
	cube->triangle(0,5,4);
	cube->triangle(1,2,6);
	cube->triangle(1,6,5);
	cube->triangle(2,3,7);
	cube->triangle(2,7,6);

	
	cube->end();
	return cube;
}

void Nave::abrirAlas(){
		
	if ( apertura < 5*15 ){
		Nave::nodoAlaDerSup->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(-0.2)));
		Nave::nodoAlaDerInf->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(0.2)));
		Nave::nodoAlaIzqSup->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(-0.2)));
		Nave::nodoAlaIzqInf->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(0.2)));

		apertura++;
	}
		
}

void Nave::cerrarAlas(){
		
	if ( apertura > 0 ){
		Nave::nodoAlaDerSup->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(0.2)));
		Nave::nodoAlaDerInf->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(-0.2)));
		Nave::nodoAlaIzqSup->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(0.2)));
		Nave::nodoAlaIzqInf->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(-0.2)));

		apertura--;
	}
		
}

void Nave::moverDerecha(){

	if ( translate >= boundry )
		return;

	printf("Posicion X: %f | translate X: %f\n",nodoNave->getPosition().x , translate);

	nodoNave->translate(0.2,0.0,0.0);

	if ( lastMove != 1 ){

		lastMove++;

		nodoNave->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(-10.0)));
	}

	translate+=0.2;
}

void Nave::moverIzquierda(){

	if ( translate <= -boundry )
		return;

	printf("Posicion X: %f | translate X: %f\n",nodoNave->getPosition().x , translate);

	nodoNave->translate(-0.2,0.0,0.0);

	if ( lastMove != -1 ){

		lastMove--;
		
		nodoNave->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(10.0)));
	}

	translate-=0.2;
}

bool Nave::moverAdelante(){

	if ( translate_z < boundry_z ){
		nodoNave->translate(0.0,0.0,-0.5);
		camera->moveRelative(Ogre::Vector3(0.0,0.0,-0.5));

		translate_z+=0.5;

		return true;
	}
	
	return false;
}

void Nave::moverAlInicio(){
	
	nodoNave->setPosition(0.0,0.0,0.0);
	camera->setPosition(Ogre::Vector3(0.0,20.0,65.0));
	camera->lookAt(Ogre::Vector3(0.0,20.0,-1.0));
	translate_z = 0.0;
	translate = 0.0;
}

void Nave::disparar(Ogre::SceneNode* nodoNave,bool motorArriba){

	float yTranslateArma = 0.0 + apertura*0.2;
	float duration = 4.0;

	if ( animState[0] == 0 ){
		
		Ogre::SceneNode* nodeLaser = _sceneManager->createSceneNode("Laser_nave0");
		nodoNave->addChild(nodeLaser);

		Ogre::Entity* laser = _sceneManager->createEntity("entLaser_nave0","usb_laser.mesh");
		laser->setMaterialName("MatLaserNave");
		nodeLaser->attachObject(laser);
		nodeLaser->setScale(0.5f,0.5f,0.5f);
		nodeLaser->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

		Ogre::Animation* animacion = _sceneManager->createAnimation("LaserNaveAnim0",duration);
		animacion->setInterpolationMode(Ogre::Animation::IM_SPLINE);

		Ogre::NodeAnimationTrack* track = animacion->createNodeTrack(0,nodeLaser);

		// Primer key de la animacion
		Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
		key->setTranslate(Ogre::Vector3(-37.0,1.5-yTranslateArma,-42.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		// Segundo key de la animacion
		key = track->createNodeKeyFrame(1.0f);
		key->setTranslate(Ogre::Vector3(-37.0,1.5-yTranslateArma,-1500.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		animState[0] = _sceneManager->createAnimationState("LaserNaveAnim0");
		animState[0]->setEnabled(true);
		animState[0]->setLoop(true);

	}

	if ( animState[1] == 0 ){
		yTranslateArma = 7.0 - apertura*0.2;

		Ogre::SceneNode* nodeLaser = _sceneManager->createSceneNode("Laser_nave1");
		nodoNave->addChild(nodeLaser);

		Ogre::Entity* laser = _sceneManager->createEntity("entLaser_nave1","usb_laser.mesh");
		laser->setMaterialName("MatLaserNave");
		nodeLaser->attachObject(laser);
		nodeLaser->setScale(0.5f,0.5f,0.5f);
		nodeLaser->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

		Ogre::Animation* animacion = _sceneManager->createAnimation("LaserNaveAnim1",duration);
		animacion->setInterpolationMode(Ogre::Animation::IM_SPLINE);

		Ogre::NodeAnimationTrack* track = animacion->createNodeTrack(0,nodeLaser);

		// Primer key de la animacion
		Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
		key->setTranslate(Ogre::Vector3(-40.0,1.5-yTranslateArma,-42.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		// Segundo key de la animacion
		key = track->createNodeKeyFrame(1.0f);
		key->setTranslate(Ogre::Vector3(-40.0,1.5-yTranslateArma,-1500.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		animState[1] = _sceneManager->createAnimationState("LaserNaveAnim1");
		animState[1]->setEnabled(true);
		animState[1]->setLoop(true);

		
	}

	if ( animState[2] == 0 ){
		yTranslateArma = 0.0 + apertura*0.2;

		Ogre::SceneNode* nodeLaser = _sceneManager->createSceneNode("Laser_nave2");
		nodoNave->addChild(nodeLaser);

		Ogre::Entity* laser = _sceneManager->createEntity("entLaser_nave2","usb_laser.mesh");
		laser->setMaterialName("MatLaserNave");
		nodeLaser->attachObject(laser);
		nodeLaser->setScale(0.5f,0.5f,0.5f);
		nodeLaser->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

		Ogre::Animation* animacion = _sceneManager->createAnimation("LaserNaveAnim2",duration);
		animacion->setInterpolationMode(Ogre::Animation::IM_SPLINE);

		Ogre::NodeAnimationTrack* track = animacion->createNodeTrack(0,nodeLaser);

		// Primer key de la animacion
		Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
		key->setTranslate(Ogre::Vector3(40.0,1.5-yTranslateArma,-42.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		// Segundo key de la animacion
		key = track->createNodeKeyFrame(1.0f);
		key->setTranslate(Ogre::Vector3(40.0,1.5-yTranslateArma,-1500.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		animState[2] = _sceneManager->createAnimationState("LaserNaveAnim2");
		animState[2]->setEnabled(true);
		animState[2]->setLoop(true);

	}

	if ( animState[3] == 0 ){
		yTranslateArma = 7.0 - apertura*0.2;

		Ogre::SceneNode* nodeLaser = _sceneManager->createSceneNode("Laser_nave3");
		nodoNave->addChild(nodeLaser);

		Ogre::Entity* laser = _sceneManager->createEntity("entLaser_nave3","usb_laser.mesh");
		laser->setMaterialName("MatLaserNave");
		nodeLaser->attachObject(laser);
		nodeLaser->setScale(0.5f,0.5f,0.5f);
		nodeLaser->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(90.0)));

		Ogre::Animation* animacion = _sceneManager->createAnimation("LaserNaveAnim3",duration);
		animacion->setInterpolationMode(Ogre::Animation::IM_SPLINE);

		Ogre::NodeAnimationTrack* track = animacion->createNodeTrack(0,nodeLaser);

		// Primer key de la animacion
		Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
		key->setTranslate(Ogre::Vector3(40.0,1.5-yTranslateArma,-42.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		// Segundo key de la animacion
		key = track->createNodeKeyFrame(1.0f);
		key->setTranslate(Ogre::Vector3(40.0,1.5-yTranslateArma,-1500.0));
		key->setScale(Ogre::Vector3(1.0,1.0,1.0));
		key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0)),Ogre::Vector3(1.0,0.0,0.0)));

		animState[3] = _sceneManager->createAnimationState("LaserNaveAnim3");
		animState[3]->setEnabled(true);
		animState[3]->setLoop(true);

		
	}

	disparando = true;

}