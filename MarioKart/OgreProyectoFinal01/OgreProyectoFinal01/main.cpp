#include "Ogre\Ogre.h"
#include "OIS\OIS.h"
#include "Font.h"
#include <time.h>


/* Variables carro */
float rapidez_carro = 0.0f;
int direccion_ruedas = 0;
bool esNave = false;

/* Animacion alas */

Ogre::Animation* animAlaIzq;
Ogre::Animation* animAlaDer;
Ogre::Animation* animRuedaIzq1;
Ogre::Animation* animRuedaIzq2;
Ogre::Animation* animRuedaDer1;
Ogre::Animation* animRuedaDer2;

Ogre::AnimationState* animStateAlaIzq;
Ogre::AnimationState* animStateAlaDer;
Ogre::AnimationState* animStateRuedaIzq1;
Ogre::AnimationState* animStateRuedaIzq2;
Ogre::AnimationState* animStateRuedaDer1;
Ogre::AnimationState* animStateRuedaDer2;

/* Colision paredes */

Ogre::SceneNode* nodeColisionCarro;
Ogre::SceneNode* _nodeChasisCarro;

/* Monedas */
int num_monedas = 40;
Ogre::SceneNode* nodo_moneda[40];
bool ya_agarradas[40];

/* Obstaculos */
int num_obstaculos_fijos = 15;
Ogre::SceneNode* nodo_obstaculo_fijo[15];

int num_obstaculos_moviles = 3;
Ogre::SceneNode* nodo_obstaculo_movil[3];
Ogre::Animation* anim_obstaculo_movil[3];
Ogre::AnimationState* anim_state_obstaculo_movil[3];

int puntos = 0;

class AppFrameListener : public Ogre::FrameListener
{

private:

	Ogre::SceneManager* _sManager;
	OIS::InputManager* _InputManager;
	OIS::Keyboard* _Keyboard;
	OIS::Mouse* _Mouse;
	Ogre::Camera* _Cam;
	float _movementspeed;
	OgreText* textMonedas;

public:

	AppFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::SceneManager* _sceneManager){

		OIS::ParamList parameters;
		unsigned int windowHandle = 0;
		std::ostringstream windowHandleString;
		win->getCustomAttribute("WINDOW", &windowHandle);
		windowHandleString << windowHandle;
		parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
		_InputManager = OIS::InputManager::createInputSystem(parameters);
		_Keyboard = static_cast<OIS::Keyboard*>(_InputManager->createInputObject( OIS::OISKeyboard, false ));
		_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject( OIS::OISMouse, false ));
		_sManager = _sceneManager;
		_Cam = cam;
		_movementspeed = 25;

		textMonedas = new OgreText();

		}
	
	~AppFrameListener(){
		_InputManager->destroyInputObject(_Keyboard);
		_InputManager->destroyInputObject(_Mouse);
		OIS::InputManager::destroyInputSystem(_InputManager);
	}

	
	std::string convertInt(int number){
		std::stringstream ss;//create a stringstream
		ss << number;//add number to the stream
		return ss.str();//return a string with the contents of the stream
	}
			
	bool intersection(float distancia, float diametro){

		if (distancia > diametro) 
			return false;
		else
			return true;


	}

	void colisionMonedas(){

		Ogre::Vector3 posCarro = _nodeChasisCarro->getPosition();

		for ( int i = 0 ; i < num_monedas ; ++i ){

			if ( !ya_agarradas[i] && intersection(posCarro.distance(nodo_moneda[i]->getPosition()) , 3.0f+nodeColisionCarro->getScale().x ) ){
				nodo_moneda[i]->setVisible(false);
				puntos++;
				ya_agarradas[i] = true;
			}

		}
	}

	bool colisionObstaculos(){

		Ogre::Vector3 posCarro = _nodeChasisCarro->getPosition();

		for ( int i = 0 ; i < num_obstaculos_fijos ; ++i ){

			if ( intersection(posCarro.distance(nodo_obstaculo_fijo[i]->getPosition()) , 3.0f+nodeColisionCarro->getScale().x ) ){
				return true;
			}
		}

		for ( int i = 0 ; i < num_obstaculos_moviles ; ++i ){

			if ( intersection(posCarro.distance(nodo_obstaculo_movil[i]->getPosition()) , 3.0f+nodeColisionCarro->getScale().x ) ){
				return true;
			}
		}

		return false;
	}

	bool frameStarted(const Ogre::FrameEvent& evt){
		
		
		textMonedas->setText("Stars: " + convertInt(puntos));
		textMonedas->setPos(0.0f,0.0f);        // Text position, using relative co-ordinates
		textMonedas->setCol(1.0f,1.0f,0.0f,0.7f);    // Text colour (Red, Green, Blue, Alpha)

		Ogre::Vector3 translate(0,0,0);
		
		Ogre::SceneNode* carro = _sManager->getSceneNode("ChasisCarro");
		
		_Keyboard->capture();
			

		if(_Keyboard->isKeyDown(OIS::KC_ESCAPE)){
			return false;
		}


		/* Camara */
		if(_Keyboard->isKeyDown(OIS::KC_W)){
			translate += Ogre::Vector3(0,0,-0.3);
		}

		if(_Keyboard->isKeyDown(OIS::KC_S)){
			translate += Ogre::Vector3(0,0,0.3);
		}

		if(_Keyboard->isKeyDown(OIS::KC_A)){
			translate += Ogre::Vector3(-0.3,0,0);
		}

		if(_Keyboard->isKeyDown(OIS::KC_D)){
			translate += Ogre::Vector3(0.3,0,0);
		}
		

		/* Animacion alas */
		if ( animStateAlaIzq != 0 && animStateAlaIzq->getEnabled() ){
			animStateAlaIzq->addTime(evt.timeSinceLastFrame);
		}
		
		if ( animStateAlaDer != 0 && animStateAlaDer->getEnabled() ){
			animStateAlaDer->addTime(evt.timeSinceLastFrame);
		}

		if ( animStateRuedaIzq1 != 0 && animStateRuedaIzq1->getEnabled() ){
			animStateRuedaIzq1->addTime(evt.timeSinceLastFrame);
		}

		if ( animStateRuedaIzq2 != 0 && animStateRuedaIzq2->getEnabled() ){
			animStateRuedaIzq2->addTime(evt.timeSinceLastFrame);
		}

		if ( animStateRuedaDer1 != 0 && animStateRuedaDer1->getEnabled() ){
			animStateRuedaDer1->addTime(evt.timeSinceLastFrame);
		}

		if ( animStateRuedaDer2 != 0 && animStateRuedaDer2->getEnabled() ){
			animStateRuedaDer2->addTime(evt.timeSinceLastFrame);
		}

		/* Animacion obstaculos */

		for (int i = 0 ; i < 3 ; ++i ){
			anim_state_obstaculo_movil[i]->addTime(evt.timeSinceLastFrame);
		}

		if ( esNave ){
			if ( _Keyboard->isKeyDown(OIS::KC_Z) ){
				carro->translate(0.0f,2.0f,0.0f);
			}
			else{
				if ( _Keyboard->isKeyDown(OIS::KC_X) ){
					carro->translate(0.0f,-2.0f,0.0f);
				}
			}
			
		}

		/* Rapidez del carro */
		bool rapidez_cambio = false;
		float max_rapidez = 10.0f;
		float min_rapidez = -5.0f;
		float cambio_rapidez = 0.1f;

		if ( _Keyboard->isKeyDown(OIS::KC_UP) ){

			if ( rapidez_carro <= max_rapidez ){
				rapidez_carro+=cambio_rapidez;
				rapidez_cambio = true;
			}
		}

		if ( _Keyboard->isKeyDown(OIS::KC_DOWN) ){
			
			if ( rapidez_carro-cambio_rapidez <= min_rapidez ){
				rapidez_carro = -5.0f;
			}
			else{
				rapidez_carro-=cambio_rapidez;
			}
			
			rapidez_cambio = true;
		}


		if ( rapidez_cambio ){
			printf("Rapidez: %f\n",rapidez_carro);
		}


		/* Direccion ruedas */
		
		bool cambio_direccion_ruedas = false;
		int max_rotacion = 10;
		int dir_ruedas = 0.0;
		int dir_carro = 0.0;

		if ( _Keyboard->isKeyDown(OIS::KC_LEFT) ){
			
			if ( direccion_ruedas > -max_rotacion ){
				direccion_ruedas--;
				dir_ruedas = -1;
				cambio_direccion_ruedas = true;
			}
			
			dir_carro = -1;
		}

		if ( _Keyboard->isKeyDown(OIS::KC_RIGHT) ){
			
			if ( direccion_ruedas < max_rotacion ){
				direccion_ruedas++;
				dir_ruedas = 1;
				cambio_direccion_ruedas = true;
			}
			
			dir_carro = 1;
		}

		if ( cambio_direccion_ruedas ){
					
			Ogre::SceneNode* ruedaDerecha2_global = _sManager->getSceneNode("RuedaDerecha2_global");
			Ogre::SceneNode* ruedaIzquierda2_global = _sManager->getSceneNode("RuedaIzquierda2_global");
		
			printf("Direccion ruedas: %d\n",direccion_ruedas);
			
			float rot = 0.0;

			if ( dir_ruedas > 0 ){
				rot = 2.0f;
			}
			else{
				if ( dir_ruedas < 0 ){
					rot = -2.0f;
				}
			}


			ruedaDerecha2_global->rotate(Ogre::Vector3(0.0f,-1.0f,0.0f),Ogre::Radian(Ogre::Degree(rot)));
			ruedaIzquierda2_global->rotate(Ogre::Vector3(0.0f,-1.0f,0.0f),Ogre::Radian(Ogre::Degree(rot)));
		}

		
		/* Animacion ruedas */

		//if ( rapidez_carro > 0.0f ){
			
		Ogre::SceneNode* ruedaDerecha1 = _sManager->getSceneNode("RuedaDerecha1");
		Ogre::SceneNode* ruedaDerecha2 = _sManager->getSceneNode("RuedaDerecha2");
		Ogre::SceneNode* ruedaIzquierda1 = _sManager->getSceneNode("RuedaIzquierda1");
		Ogre::SceneNode* ruedaIzquierda2 = _sManager->getSceneNode("RuedaIzquierda2");
		
		Ogre::Radian rotation = Ogre::Radian(0.1f * rapidez_carro );
		
		if ( !esNave ){
			ruedaDerecha1->rotate(Ogre::Vector3(1.0f,0.0f,0.0f),rotation);
			ruedaDerecha2->rotate(Ogre::Vector3(1.0f,0.0f,0.0f),rotation);
			ruedaIzquierda1->rotate(Ogre::Vector3(1.0f,0.0f,0.0f),rotation);
			ruedaIzquierda2->rotate(Ogre::Vector3(1.0f,0.0f,0.0f),rotation);
		}

		/* Movimiento del carro */

		float limites[3];
		limites[0] = 120.0f;
		limites[1] = 200.0f;
		limites[2] = 20.0f;

		carro->translate(0.0f,0.0f,rapidez_carro);

		float carro_z = carro->getPosition().z;
		float carro_x = carro->getPosition().x;
		
		//printf("CARRO_Z: %f | CARRO_X: %f\n",carro_z,carro_x);

		if ( !esNave && carro_z >= 6500 ){
			transformarEnNave(_sManager);
		}

		if ( carro_z >= -5.0f && carro_z <= 403.0f ){
			if ( (direccion_ruedas < 0 && carro_x < limites[0]) || (direccion_ruedas > 0 && carro_x > -limites[0]) ){
				carro->translate(-direccion_ruedas*0.5f,0.0f,0.0f);
			}
			else{
				carro->translate(direccion_ruedas*0.5f,0.0f,0.0f);
			}
		}
		else{
			if ( (carro_z > 403.0f && carro_z <= 2351.0f) || (carro_z > 5505.0f && carro_z <= 6535.0f ) ){
				if ( (direccion_ruedas < 0 && carro_x < limites[1]) || (direccion_ruedas > 0 && carro_x > -limites[1]) ){
					carro->translate(-direccion_ruedas*0.5f,0.0f,0.0f);
				}
				else{
					carro->translate(direccion_ruedas*0.5f,0.0f,0.0f);
				}
			}
			else{
				if ( carro_z > 2927.0f && carro_z <= 4929.0f ){
					if ( (direccion_ruedas < 0 && carro_x < limites[2]) || (direccion_ruedas > 0 && carro_x > -limites[2]) ){
						carro->translate(-direccion_ruedas*0.5f,0.0f,0.0f);
					}
					else{
						carro->translate(direccion_ruedas*0.5f,0.0f,0.0f);
					}
				}
				else{
					if ( carro_z > 2351.0f && carro_z <= 2927.0f ){
						//Colisiono con las primeras

						float diff = carro_z - 2351.0f;
						float limite_1 = (limites[1]-13.0f)-diff*0.3f;

						if ( direccion_ruedas < 0 ){
							printf("carro_x: %f < limite: %f\n",carro_x,limite_1);
						}
						else{
							printf("carro_x: %f > limite: %f\n",carro_x,limite_1);
						}

						if ( (direccion_ruedas < 0 && carro_x < limite_1 ) || (direccion_ruedas > 0 && carro_x > -limite_1 ) ){
							carro->translate(-direccion_ruedas*0.5f,0.0f,0.0f);
						}
						else{
							carro->translate(direccion_ruedas*0.5f,0.0f,0.0f);
						}
					}
					else{
						if ( carro_z > 4929.0f && carro_z <= 5505.0f ){
							//Colisiono con las ultimas
							float diff = carro_z - 4929.0f;
							float limite_1 = (limites[2]-13.0f)+diff*0.3f;

							if ( (direccion_ruedas < 0 && carro_x < limite_1 ) || (direccion_ruedas > 0 && carro_x > -limite_1 ) ){
								carro->translate(-direccion_ruedas*0.5f,0.0f,0.0f);
							}
							else{
								carro->translate(direccion_ruedas*0.5f,0.0f,0.0f);
							}
							
						}
						else{
							carro->translate(-direccion_ruedas*0.5f,0.0f,0.0f);
						}
					}
				}
			}
		}

		colisionMonedas();

		if ( colisionObstaculos() ){
			rapidez_carro = -1.5f;
			puntos--;
		}
		
		if ( dir_carro > 0 ){


			if ( carro_z >= -5.0f && carro_z <= 403.0f && carro_x < limites[0] ){
				carro->translate(-1.0f,0.0f,0.0f);
			}
		}
		else{
			if ( dir_carro < 0 ){
				if ( carro_z >= 0.0f && carro_z <= 403.0f && carro_x > -limites[0] ){
					carro->translate(1.0f,0.0f,0.0f);
				}
			}
		}
		//}

		if ( carro_z > 2927.0f && carro_z < 2927.0f + 50.0f ){
			translate += Ogre::Vector3(0,0,-0.3);
		}
		else{
			if ( carro_z > 4929.0f-50.0f && carro_z < 4929.0f ){
				translate += Ogre::Vector3(0,0,0.3);
			}
			else{
				if ( carro_z > 9700.0f && carro_z < 10100.0f+1000.0f ){
					translate += Ogre::Vector3(0,0,0.2);
				}
			}
		}

		
		_Cam->moveRelative(translate*evt.timeSinceLastFrame * 600);
		

		_Mouse->capture();
		float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame* -1;
		float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
		_Cam->yaw(Ogre::Radian(rotX));
		_Cam->pitch(Ogre::Radian(rotY));

		return true;
	}
	
	bool frameEnded(const Ogre::FrameEvent& evt){
		//std::cout << "Frame ended" << std::endl;
		return true;
	}

	bool frameRenderingQueued(const Ogre::FrameEvent& evt){
		//std::cout << "Frame queued" << std::endl;
		return true;
	}

	void transformarEnNave( Ogre::SceneManager* _sceneManager ){

		animStateAlaIzq->setEnabled(true);
		animStateAlaDer->setEnabled(true);
		animStateRuedaIzq1->setEnabled(true);
		animStateRuedaIzq2->setEnabled(true);
		animStateRuedaDer1->setEnabled(true);
		animStateRuedaDer2->setEnabled(true);

		esNave = true;

	}

};

Ogre::ManualObject* crearAla(Ogre::String name, Ogre::String matName) {
	
	float ancho_ala = -1.0f;
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

class cAplicacion {

	private:
		Ogre::SceneManager* _sceneManager;
		Ogre::Root* _root;
		AppFrameListener* _listener;
		Ogre::Camera* camera;
		bool _keepRunning;
		

		public:

			// Constructor de la clase
			cAplicacion(){
				_sceneManager = NULL;
				_root = NULL;
				_listener = NULL;
			}

			~cAplicacion(){
				delete _root;
				delete _listener;
			}

			// Funcion que permite cargar los recursos de OGRE esto simplifica tener que cargarlos uno por uno
			void loadResources(){
				Ogre::ConfigFile cf;
				cf.load("resources_d.cfg");

				Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
				Ogre::String sectionName, typeName, dataname;
				
				while (sectionIter.hasMoreElements()){
					sectionName = sectionIter.peekNextKey();
					Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
					Ogre::ConfigFile::SettingsMultiMap::iterator i;

					for (i = settings->begin(); i != settings->end(); ++i){
						typeName = i->first;
						dataname = i->second;
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);
					}
				
				}

				Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
			}


			// funcion que configura la camara
			void createCamera(Ogre::RenderWindow* window){

				Ogre::SceneNode* cam_node = _sceneManager->createSceneNode("cam_node");

				camera = _sceneManager->createCamera("Camera");
				camera->setPosition(Ogre::Vector3(0,60,-100));
				camera->lookAt(Ogre::Vector3(0,0,50));
				camera->setNearClipDistance(5);

				Ogre::Viewport* viewport = window->addViewport(camera);
				viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
				camera->setAspectRatio(Ogre::Real(viewport->getActualWidth())/Ogre::Real(viewport->getActualHeight()));

				cam_node->attachObject(camera);
				_sceneManager->getSceneNode("ChasisCarro")->addChild(cam_node);

			}

			// funcion donde se coloca lo que se desea desplegar.
			void createScene(){

				for ( int i = 0 ; i < num_monedas ; ++i ){
					ya_agarradas[i] = false;
				}

				_sceneManager->setAmbientLight(Ogre::ColourValue(0.2,0.2,0.2));
				_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
				
				_sceneManager->setSkyDome(true, "Examples/SpaceSkyPlane", 5, 8);

				// LUZ
				Ogre::Light* LuzPuntual01 = _sceneManager->createLight("Luz01");
				LuzPuntual01->setType(Ogre::Light::LT_DIRECTIONAL);
				LuzPuntual01->setDiffuseColour(1.0,1.0,1.0);
				LuzPuntual01->setPosition(Ogre::Vector3(0.0,0.0,-1.0));
				LuzPuntual01->setDirection(Ogre::Vector3( 1, -1, 1 ));

				Ogre::Light* LuzPuntual02 = _sceneManager->createLight("Luz02");
				LuzPuntual02->setType(Ogre::Light::LT_DIRECTIONAL);
				LuzPuntual02->setDiffuseColour(1.0,1.0,1.0);
				LuzPuntual02->setPosition(Ogre::Vector3(0.0,0.0,-1.0));
				LuzPuntual02->setDirection(Ogre::Vector3( -1, -1, -1 ));

				/*Ogre::Light* LuzPuntual03 = _sceneManager->createLight("Luz03");
				LuzPuntual03->setType(Ogre::Light::LT_DIRECTIONAL);
				LuzPuntual03->setDiffuseColour(1.0,1.0,1.0);
				LuzPuntual03->setPosition(Ogre::Vector3(0.0,0.0,10000.0));
				LuzPuntual03->setDirection(Ogre::Vector3( -1, -1, 1 ));/*

				/* Luz del tunel */

				Ogre::Light* LucesTunel[10];
				float posicion = 3050.0f;
				for (int i = 0; i < 10; ++i) {

					LucesTunel[i] = _sceneManager->createLight();
					LucesTunel[i]->setType(Ogre::Light::LT_POINT);
					LucesTunel[i]->setDiffuseColour(50.0,50.0,50.0);
					LucesTunel[i]->setPosition(0.0f,10.0f,posicion);
					LucesTunel[i]->setCastShadows(false);
					LucesTunel[i]->setAttenuation(65, 1.0, 0.07, 0.017);

					posicion = posicion + 200.0f;
				}


				dibujarPista();
				dibujarCarro();
				colocarMonedas();
				colocarObstaculos();

				/* Contador de puntos */
				/*OgreText *textItem = new OgreText();
				textItem->setText("Hello World!");
				textItem->setPos(0.1f,0.1f);        // Text position, using relative co-ordinates
				textItem->setCol(1.0f,1.0f,1.0f,0.5f);    // Text colour (Red, Green, Blue, Alpha)*/
			}

			void colocarMonedas(){

				// Primer tramo pista
				for( int i = 0 ; i < 12 ; ++i ){
					crearMoneda(i,200,50,2351);
				}

				// Tunel
				for( int i = 12 ; i < 21 ; ++i ){
					crearMoneda(i,32,2927,4929);
				}
				
				// Segundo tramo pista
				for( int i = 21 ; i < 33 ; ++i ){
					crearMoneda(i,200,5505,2351);
				}
				
				// Espacio abierto
				for( int i = 33 ; i < 40 ; ++i ){
					crearMoneda(i,200,6535,10305);
				}

			}
			
			void colocarObstaculos(){

				// Primera parte
				for ( int i = 0 ; i < 5 ; ++i ){
					crearObstaculo(i,150,403,904);
				}

				// Segunda parte
				for ( int i = 5 ; i < 10 ; ++i ){
					crearObstaculo(i,150,1320,2351);
				}

				// Tercera parte
				for ( int i = 10 ; i < 15 ; ++i ){
					crearObstaculo(i,150,5505,6535);
				}

				srand(time(NULL));

				crearObstaculoMovil(0,200,403,904);
				crearObstaculoMovil(1,200,1320,2551);

				crearObstaculoMovil(2,200,5505,6535);
			}

			void crearObstaculo( int num , int limite_x , int limite_min_z , int limite_max_z ){

				
				float pos_x = rand() % limite_x;

				if ( rand() % 2 == 1 ){
					pos_x*=-1;
				}

				float pos_z = (rand() % (limite_max_z-limite_min_z))+limite_min_z;

				nodo_obstaculo_fijo[num] = _sceneManager->createSceneNode("ObstaculoFijoB_" + convertInt(num) );
				Ogre::SceneNode* _nodoObstaculoA = _sceneManager->createSceneNode("ObstaculoFijoA_" + convertInt(num) );
				
				Ogre::Entity* _entObstaculoB = _sceneManager->createEntity("cilindro01.mesh");				
				Ogre::Entity* _entObstaculoA = _sceneManager->createEntity("cilindro01.mesh");

				_entObstaculoB->setMaterialName("MatVerde");
				_entObstaculoA->setMaterialName("MatVerde");
				
				_nodoObstaculoA->attachObject(_entObstaculoA);
				nodo_obstaculo_fijo[num]->attachObject(_entObstaculoB);

				nodo_obstaculo_fijo[num]->setPosition(pos_x,0.0f,pos_z);
				nodo_obstaculo_fijo[num]->setScale(1.2f,1.0f,1.0f);
				_nodoObstaculoA->setPosition(0.0,19.0,0.0);
				_nodoObstaculoA->setScale(1.5f,0.15f,1.5f);

				nodo_obstaculo_fijo[num]->addChild(_nodoObstaculoA);
				nodo_obstaculo_fijo[num]->setScale(2.0f,2.0f,2.0f);

				_sceneManager->getRootSceneNode()->addChild(nodo_obstaculo_fijo[num]);
			}

			void crearObstaculoMovil( int num , int limite_x , int limite_min_z , int limite_max_z ){
				
				float pos_x = rand() % limite_x;
				bool right = false;

				if ( rand() % 2 == 1 ){
					pos_x*=-1;
					right = true;
				}

				float pos_z = (rand() % (limite_max_z-limite_min_z))+limite_min_z;
				
				nodo_obstaculo_movil[num] = _sceneManager->createSceneNode("nodoBomba" + convertInt(num) );
				Ogre::SceneNode* _nodoPieIzq = _sceneManager->createSceneNode("nodoPieIzq" + convertInt(num));
				Ogre::SceneNode* _nodoPieDer = _sceneManager->createSceneNode("nodoPieDer" + convertInt(num));
				Ogre::SceneNode* _nodoChimenea = _sceneManager->createSceneNode("nodoChimenea" + convertInt(num));
				Ogre::SceneNode* _nodoCuerda = _sceneManager->createSceneNode("nodoCuerda" + convertInt(num));
				Ogre::SceneNode* _nodoOjoDer = _sceneManager->createSceneNode("nodoOjoDer" + convertInt(num));
				Ogre::SceneNode* _nodoOjoIzq = _sceneManager->createSceneNode("nodoOjoIzq" + convertInt(num));

				Ogre::Entity* _entBomba = _sceneManager->createEntity("esfera.mesh");				
				Ogre::Entity* _entPieIzq = _sceneManager->createEntity("esfera.mesh");
				Ogre::Entity* _entPieDer = _sceneManager->createEntity("esfera.mesh");				
				Ogre::Entity* _entChimenea = _sceneManager->createEntity("cilindro01.mesh");
				Ogre::Entity* _entCuerda = _sceneManager->createEntity("cilindro01.mesh");
				Ogre::Entity* _entOjoIzq = _sceneManager->createEntity("cilindro01.mesh");
				Ogre::Entity* _entOjoDer = _sceneManager->createEntity("cilindro01.mesh");

				_entBomba->setMaterialName("MatNegro");
				_entPieIzq->setMaterialName("MatRojo");
				_entPieDer->setMaterialName("MatRojo");
				_entChimenea->setMaterialName("MatNegro");
				_entCuerda->setMaterialName("MatBlanco"); 
				_entOjoIzq->setMaterialName("MatBlanco"); 
				_entOjoDer->setMaterialName("MatBlanco"); 

				nodo_obstaculo_movil[num]->attachObject(_entBomba);
				_nodoPieIzq->attachObject(_entPieIzq);
				_nodoPieDer->attachObject(_entPieDer);
				_nodoChimenea->attachObject(_entChimenea);
				_nodoCuerda->attachObject(_entCuerda);
				_nodoOjoDer->attachObject(_entOjoDer);
				_nodoOjoIzq->attachObject(_entOjoIzq);
				
				nodo_obstaculo_movil[num]->setPosition(pos_x,7.0f,pos_z);
				nodo_obstaculo_movil[num]->setScale(7.0f,7.0f,7.0f);
				
				_nodoPieIzq->setPosition(0.7,-1.0,0.0);
				_nodoPieIzq->setScale(0.5f,0.025f,0.5f);
				_nodoPieDer->setPosition(-0.7,-1.0,0.0);
				_nodoPieDer->setScale(0.5f,0.025f,0.5f);

				_nodoChimenea->setScale(0.05,0.025,0.05);
				_nodoChimenea->setPosition(0.0,1.0,0.0);
				
				_nodoCuerda->setScale(0.01,0.01,0.01);
				_nodoCuerda->setPosition(0.0,1.5,0.0);

				_nodoOjoDer->setScale(0.03,0.02,0.03);
				_nodoOjoIzq->setScale(0.03,0.02,0.03);
				_nodoOjoDer->setPosition(-0.3,0.0,-1.0);
				_nodoOjoIzq->setPosition(0.3,0.0,-1.0);


				nodo_obstaculo_movil[num]->addChild(_nodoPieIzq);
				nodo_obstaculo_movil[num]->addChild(_nodoPieDer);
				nodo_obstaculo_movil[num]->addChild(_nodoChimenea);
				nodo_obstaculo_movil[num]->addChild(_nodoCuerda);
				nodo_obstaculo_movil[num]->addChild(_nodoOjoDer);
				nodo_obstaculo_movil[num]->addChild(_nodoOjoIzq);				
				
				_sceneManager->getRootSceneNode()->addChild(nodo_obstaculo_movil[num]);

				anim_obstaculo_movil[num] = _sceneManager->createAnimation("AnimObstaculo" + convertInt(num),5.0f);
				anim_obstaculo_movil[num]->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track = anim_obstaculo_movil[num]->createNodeTrack(0,nodo_obstaculo_movil[num]);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
				key->setScale(Ogre::Vector3(7.0f,7.0f,7.0f));
				key->setTranslate(nodo_obstaculo_movil[num]->getPosition());

				// Segundo key de la animacion
				key = track->createNodeKeyFrame(2.25f);
				key->setScale(Ogre::Vector3(7.0f,7.0f,7.0f));
				key->setTranslate(Ogre::Vector3(right? limite_x:-limite_x,5.0f,pos_z));

				// Tercer key de la animacion
				key = track->createNodeKeyFrame(3.75f);
				key->setScale(Ogre::Vector3(7.0f,7.0f,7.0f));
				key->setTranslate(Ogre::Vector3(right? -limite_x:limite_x,5.0f,pos_z));

				// Cuarto key de la animacion
				key = track->createNodeKeyFrame(5.0f);
				key->setScale(Ogre::Vector3(7.0f,7.0f,7.0f));
				key->setTranslate(nodo_obstaculo_movil[num]->getPosition());
				
				anim_state_obstaculo_movil[num] = _sceneManager->createAnimationState("AnimObstaculo" + convertInt(num));
				anim_state_obstaculo_movil[num]->setEnabled(true);
				anim_state_obstaculo_movil[num]->setLoop(true);
			}

			std::string convertInt(int number){
				std::stringstream ss;//create a stringstream
				ss << number;//add number to the stream
				return ss.str();//return a string with the contents of the stream
			}
			
			void crearMoneda( int num , int limite_x , int limite_min_z , int limite_max_z ){
				
				float pos_x = rand() % limite_x;

				if ( rand() % 2 == 1 ){
					pos_x*=-1;
				}

				float pos_z = (rand() % (limite_max_z-limite_min_z))+limite_min_z;

				nodo_moneda[num] = _sceneManager->createSceneNode("Moneda" + convertInt(num));
				_sceneManager->getRootSceneNode()->addChild(nodo_moneda[num]);
				
				Ogre::Entity* _entMoneda = _sceneManager->createEntity("cilindro01.mesh");
				_entMoneda->setMaterialName("MatMoneda");
				nodo_moneda[num]->setPosition(pos_x,10.0,pos_z);
				nodo_moneda[num]->setScale(1.5f,0.1f,1.5f);
				nodo_moneda[num]->rotate(Ogre::Vector3(1.0,0.0,0.0), Ogre::Radian(Ogre::Degree(90.0)));
				nodo_moneda[num]->attachObject(_entMoneda);
			}

			void dibujarCarro(){
				
				//Chasis Carro
				_nodeChasisCarro = _sceneManager->createSceneNode("ChasisCarro");
				_sceneManager->getRootSceneNode()->addChild(_nodeChasisCarro);
				_nodeChasisCarro->translate(0.0f,0.0f,-5.0f);
				
				Ogre::Entity* _entChasisCarro = _sceneManager->createEntity("ChasisCarro01", "chasisCarro.mesh");
				_nodeChasisCarro->attachObject(_entChasisCarro);

				/* Particle system */
				Ogre::ParticleSystem* partSystem = _sceneManager->createParticleSystem("humo","Humo");
				_nodeChasisCarro->attachObject(partSystem);

				float separacion_carro_rueda = 5.3f;
				float separacion_rueda_rueda = 15.0f;
				float inicio_ruedas_traseras = 9.5f;
				float elevacion = 3.6f;
				float ancho_rueda = 2.0f;

				/* Rueda derecha 1 */
				Ogre::SceneNode* nodeRuedaDerecha1 = _sceneManager->createSceneNode("RuedaDerecha1");
				nodeRuedaDerecha1->translate(-separacion_carro_rueda,elevacion,-inicio_ruedas_traseras);
				_nodeChasisCarro->addChild(nodeRuedaDerecha1);				

				Ogre::Entity* entRuedaDerecha1 = _sceneManager->createEntity("EntRuedaDerecha1", "ruedaDetallada.mesh");
				nodeRuedaDerecha1->attachObject(entRuedaDerecha1);

				/* Rueda derecha 2 */
				Ogre::SceneNode* nodeRuedaDerecha2_global = _sceneManager->createSceneNode("RuedaDerecha2_global");				

				Ogre::SceneNode* nodeRuedaDerecha2 = _sceneManager->createSceneNode("RuedaDerecha2");
				nodeRuedaDerecha2_global->translate(-separacion_carro_rueda,elevacion,inicio_ruedas_traseras);
				nodeRuedaDerecha2_global->addChild(nodeRuedaDerecha2);
				_nodeChasisCarro->addChild(nodeRuedaDerecha2_global);

				Ogre::Entity* entRuedaDerecha2 = _sceneManager->createEntity("EntRuedaDerecha2", "ruedaDetallada.mesh");
				nodeRuedaDerecha2->attachObject(entRuedaDerecha2);

				/* Rueda izquierda 1 */
				Ogre::SceneNode* nodeRuedaIzquierda1 = _sceneManager->createSceneNode("RuedaIzquierda1");
				nodeRuedaIzquierda1->translate(separacion_carro_rueda+ancho_rueda,elevacion,-inicio_ruedas_traseras);
				_nodeChasisCarro->addChild(nodeRuedaIzquierda1);	

				Ogre::Entity* entRuedaIzquierda1 = _sceneManager->createEntity("EntRuedaIzquierda1", "ruedaDetallada.mesh");
				nodeRuedaIzquierda1->attachObject(entRuedaIzquierda1);

				/* Rueda izquierda 2 */
				Ogre::SceneNode* nodeRuedaIzquierda2_global = _sceneManager->createSceneNode("RuedaIzquierda2_global");

				Ogre::SceneNode* nodeRuedaIzquierda2 = _sceneManager->createSceneNode("RuedaIzquierda2");
				nodeRuedaIzquierda2_global->translate(separacion_carro_rueda+ancho_rueda,elevacion,inicio_ruedas_traseras);
				nodeRuedaIzquierda2_global->addChild(nodeRuedaIzquierda2);
				_nodeChasisCarro->addChild(nodeRuedaIzquierda2_global);

				Ogre::Entity* entRuedaIzquierda2 = _sceneManager->createEntity("EntRuedaIzquierda2", "ruedaDetallada.mesh");
				nodeRuedaIzquierda2->attachObject(entRuedaIzquierda2);

				/* Ala izquierda */				
				Ogre::SceneNode* nodeAlaIzq = _sceneManager->createSceneNode("NodeAlaIzq");
				Ogre::ManualObject* alaIzq = crearAla("AlaIzquierda","MatPropio01");
				nodeAlaIzq->translate(0.0f,7.5f,3.0f);
				nodeAlaIzq->rotate(Ogre::Vector3(0.0f,0.0f,1.0f),Ogre::Radian(Ogre::Degree(180.0f)));
				nodeAlaIzq->setScale(0.01f,0.01f,0.01f);
				nodeAlaIzq->attachObject(alaIzq);
				_nodeChasisCarro->addChild(nodeAlaIzq);

				/* Ala derecha */				
				Ogre::SceneNode* nodeAlaDer = _sceneManager->createSceneNode("NodeAlaDer");
				Ogre::ManualObject* alaDer = crearAla("AlaDerecha","MatPropio01");
				nodeAlaDer->translate(0.0f,7.5f,3.0f);
				nodeAlaDer->setScale(0.01f,0.01f,0.01f);
				nodeAlaDer->attachObject(alaDer);
				_nodeChasisCarro->addChild(nodeAlaDer);


				/* Colision carro */
				nodeColisionCarro = _sceneManager->createSceneNode("nodeColisionCarro");
				Ogre::Entity* esferaColisionCarro = _sceneManager->createEntity("esferaColisionCarro", "esfera.mesh");
				esferaColisionCarro->setMaterialName("MatEsferaColision");
				nodeColisionCarro->setScale(12.0f,12.0f,12.0f);
				nodeColisionCarro->attachObject(esferaColisionCarro);
				nodeColisionCarro->setVisible(false);
				
				_nodeChasisCarro->addChild(nodeColisionCarro);
			}

			void dibujarPista(){

				// Borde pista
				Ogre::SceneNode* _nodeBPista = _sceneManager->createSceneNode("BordePista");
				_sceneManager->getRootSceneNode()->addChild(_nodeBPista);
				
				Ogre::Entity* _entBPista = _sceneManager->createEntity("BordePista01", "bordePista03.mesh");
				_entBPista->setMaterialName("MatPared");
				_nodeBPista->attachObject(_entBPista);
				
				//PisoObstaculo
				Ogre::SceneNode* _nodePObstaculo = _sceneManager->createSceneNode("PistaObstaculo");
				_sceneManager->getRootSceneNode()->addChild(_nodePObstaculo);
				
				Ogre::Entity* _entPObstaculo = _sceneManager->createEntity("PistaObstaculo", "pisoObstaculo02.mesh");
				_entPObstaculo->setMaterialName("MatPiso");
				_nodePObstaculo->attachObject(_entPObstaculo);

				//PisoNOObstaculo
				Ogre::SceneNode* _nodePNObstaculo = _sceneManager->createSceneNode("PistaNoObstaculo");
				_sceneManager->getRootSceneNode()->addChild(_nodePNObstaculo);
				
				Ogre::Entity* _entPNOObstaculo = _sceneManager->createEntity("PistaNoObstaculo", "pisoNoObstaculo02.mesh");
				_entPNOObstaculo->setMaterialName("MatPiso");
				_nodePNObstaculo->attachObject(_entPNOObstaculo);


				//PosterInicioFinal
				Ogre::SceneNode* _nodePoster = _sceneManager->createSceneNode("PosterInicioFinal");
				_sceneManager->getRootSceneNode()->addChild(_nodePoster);
				
				Ogre::Entity* _entPoster = _sceneManager->createEntity("PosterInicioFinal", "posterInicioFinal.mesh");
				_entPoster->setMaterialName("MatPoste");
				_nodePoster->attachObject(_entPoster);

				
				//BanderaInicial
				Ogre::SceneNode* _nodeBInicial = _sceneManager->createSceneNode("BanderaInicial");
				_sceneManager->getRootSceneNode()->addChild(_nodeBInicial);
				
				Ogre::Entity* _entBanderaI = _sceneManager->createEntity("BanderaInicial", "banderaInicial02.mesh");
				_entBanderaI->setMaterialName("MatBanderaInicio");
				_nodeBInicial->attachObject(_entBanderaI);

				//BanderaFinal
				Ogre::SceneNode* _nodeBFinal = _sceneManager->createSceneNode("BanderaFinal");
				_sceneManager->getRootSceneNode()->addChild(_nodeBFinal);
				
				Ogre::Entity* _entBanderaF = _sceneManager->createEntity("BanderaFinal", "banderaFinal02.mesh");
				_entBanderaF->setMaterialName("MatBanderaFinal");
				_nodeBFinal->attachObject(_entBanderaF);
				
			}

			void renderOneFrame(){
				Ogre::WindowEventUtilities::messagePump();
				_keepRunning = _root->renderOneFrame();
			}

			bool keepRunning(){
				return _keepRunning;
			}

			void crearAnimaciones(){
				
				float ancho_rueda = 2.0f;

				/* Animacion Ala Izquierda */
				animAlaIzq = _sceneManager->createAnimation("AlaIzqAnim",2.0f);
				Ogre::SceneNode* alaIzq = _sceneManager->getSceneNode("NodeAlaIzq");		

				animAlaIzq->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track = animAlaIzq->createNodeTrack(0,alaIzq);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(0.0f);
				key->setScale(Ogre::Vector3(0.05,0.05,0.05));
				key->setTranslate(alaIzq->getPosition());
				key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(180.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));

				// Segundo key de la animacion
				key = track->createNodeKeyFrame(2.0f);
				key->setScale(Ogre::Vector3(0.5,0.5,0.5));
				key->setTranslate(alaIzq->getPosition());
				key->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(180.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				
				animStateAlaIzq = _sceneManager->createAnimationState("AlaIzqAnim");
				animStateAlaIzq->setEnabled(false);
				animStateAlaIzq->setLoop(false);

				/* Animacion Ala derecha */
				animAlaDer = _sceneManager->createAnimation("AlaDerAnim",2.0f);
				Ogre::SceneNode* alaDer = _sceneManager->getSceneNode("NodeAlaDer");		

				animAlaDer->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track2 = animAlaDer->createNodeTrack(1,alaDer);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key2 = track2->createNodeKeyFrame(0.0f);
				key2->setScale(Ogre::Vector3(0.05,0.05,0.05));
				key2->setTranslate(alaDer->getPosition());
				
				// Segundo key de la animacion
				key2 = track2->createNodeKeyFrame(2.0f);
				key2->setScale(Ogre::Vector3(0.5,0.5,0.5));
				key2->setTranslate(alaDer->getPosition());
				
				animStateAlaDer = _sceneManager->createAnimationState("AlaDerAnim");
				animStateAlaDer->setEnabled(false);
				animStateAlaDer->setLoop(false);

				/* Animacion rueda izquierda 1 */
				
				animRuedaIzq1 = _sceneManager->createAnimation("RuedaIzq1Anim",2.0f);
				Ogre::SceneNode* ruedaIzq1 = _sceneManager->getSceneNode("RuedaIzquierda1");		

				animRuedaIzq1->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track3 = animRuedaIzq1->createNodeTrack(2,ruedaIzq1);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key3 = track3->createNodeKeyFrame(0.0f);
				key3->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				key3->setTranslate(ruedaIzq1->getPosition());
				
				// Segundo key de la animacion
				key3 = track3->createNodeKeyFrame(2.0f);

				Ogre::Vector3 posRuedaIzq1(ruedaIzq1->getPosition());
				posRuedaIzq1.x-=ancho_rueda;

				key3->setTranslate(posRuedaIzq1);
				key3->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				
				animStateRuedaIzq1 = _sceneManager->createAnimationState("RuedaIzq1Anim");
				animStateRuedaIzq1->setEnabled(false);
				animStateRuedaIzq1->setLoop(false);

				/* Animacion rueda izquierda 2 */
				animRuedaIzq2 = _sceneManager->createAnimation("RuedaIzq2Anim",2.0f);
				Ogre::SceneNode* ruedaIzq2 = _sceneManager->getSceneNode("RuedaIzquierda2_global");		

				animRuedaIzq2->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track5 = animRuedaIzq2->createNodeTrack(4,ruedaIzq2);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key5 = track5->createNodeKeyFrame(0.0f);
				key5->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				key5->setTranslate(ruedaIzq2->getPosition());
				
				// Segundo key de la animacion
				key5 = track5->createNodeKeyFrame(2.0f);
				Ogre::Vector3 posRuedaIzq2(ruedaIzq2->getPosition());
				posRuedaIzq2.x-=ancho_rueda;

				key5->setTranslate(posRuedaIzq2);
				key5->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(90.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				
				animStateRuedaIzq2 = _sceneManager->createAnimationState("RuedaIzq2Anim");
				animStateRuedaIzq2->setEnabled(false);
				animStateRuedaIzq2->setLoop(false);

				/* Animacion rueda derecha 1 */
				
				animRuedaDer1 = _sceneManager->createAnimation("RuedaDer1Anim",2.0f);
				Ogre::SceneNode* ruedaDer1 = _sceneManager->getSceneNode("RuedaDerecha1");		

				animRuedaDer1->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track4 = animRuedaDer1->createNodeTrack(3,ruedaDer1);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key4 = track4->createNodeKeyFrame(0.0f);
				key4->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				key4->setTranslate(ruedaDer1->getPosition());
				
				// Segundo key de la animacion
				key4 = track4->createNodeKeyFrame(2.0f);

				Ogre::Vector3 posRuedaDer1(ruedaDer1->getPosition());
				posRuedaDer1.y-=ancho_rueda;

				key4->setTranslate(posRuedaDer1);
				//key4->setTranslate(Ogre::Vector3(0.0f,ancho_rueda,0.0f));
				key4->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				
				animStateRuedaDer1 = _sceneManager->createAnimationState("RuedaDer1Anim");
				animStateRuedaDer1->setEnabled(false);
				animStateRuedaDer1->setLoop(false);

				/* Animacion rueda derecha 2 */
				animRuedaDer2 = _sceneManager->createAnimation("RuedaDer2Anim",2.0f);
				Ogre::SceneNode* ruedaDer2 = _sceneManager->getSceneNode("RuedaDerecha2_global");		

				animRuedaDer2->setInterpolationMode(Ogre::Animation::IM_SPLINE);

				Ogre::NodeAnimationTrack* track6 = animRuedaDer2->createNodeTrack(4,ruedaDer2);

				// Primer key de la animacion
				Ogre::TransformKeyFrame* key6 = track6->createNodeKeyFrame(0.0f);
				key6->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				key6->setTranslate(ruedaDer2->getPosition());
				
				// Segundo key de la animacion
				key6 = track6->createNodeKeyFrame(2.0f);
				Ogre::Vector3 posRuedaDer2(ruedaDer2->getPosition());
				posRuedaDer2.y-=ancho_rueda;

				key6->setTranslate(posRuedaDer2);
				key6->setRotation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90.0f)),Ogre::Vector3(0.0f,0.0f,1.0f)));
				
				animStateRuedaDer2 = _sceneManager->createAnimationState("RuedaDer2Anim");
				animStateRuedaDer2->setEnabled(false);
				animStateRuedaDer2->setLoop(false);

			}

			//Funcion que inicializa la aplicacion
			int startup(){
				_root = new Ogre::Root("plugins_d.cfg");
				
				//Muestra la ventana inicial...
				
				//if(!_root->showConfigDialog()){
				//	return -1;
				//}


				// Evitan cargar la ventana inicial...
				Ogre::RenderSystem* _rs = _root->getRenderSystemByName("Direct3D9 Rendering Subsystem");
				// or use "OpenGL Rendering Subsystem"
				_root->setRenderSystem(_rs);
				_rs->setConfigOption("Full Screen", "No");
				_rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
				_rs->setConfigOption("FSAA", "0");
				_rs->setConfigOption("Floating-point mode", "Fastest");
				_rs->setConfigOption("Use Multihead", "Auto");
				_rs->setConfigOption("VSync", "No");
				_rs->setConfigOption("VSync Interval", "1");


			
				Ogre::RenderWindow* window = _root->initialise(true,"Ventana de Ogre3D");
				_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);

				
				loadResources();
				createScene();
				createCamera(window);

				crearAnimaciones();


				_listener = new AppFrameListener(window,camera,_sceneManager);
				_root->addFrameListener(_listener);				


				//_root->startRendering();
				return 0;

				
			}


};

int main (void)
{
	cAplicacion app;
	app.startup();

	while(app.keepRunning()){
		app.renderOneFrame();
	}

	return 0;
		
}