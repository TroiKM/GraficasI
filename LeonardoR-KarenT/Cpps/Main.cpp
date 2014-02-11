#include "Ogre\Ogre.h"
#include "OIS\OIS.h"
#include "Nave.h"
#include "Torreta.h"

Nave* nave;
Torreta* torretas[4];
Ogre::SceneNode* camara_nave;

class AppFrameListener : public Ogre::FrameListener{

private:

OIS::InputManager* _InputManager;
OIS::Keyboard* _Keyboard;
OIS::Mouse* _Mouse;
Ogre::Camera* _Cam;
float _movementspeed;

Ogre::SceneManager* _sceneManager;

// Animacion


float apertura;

public:

	AppFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam , Ogre::SceneManager* sm ){

		OIS::ParamList parameters;
		unsigned int windowHandle = 0;
		std::ostringstream windowHandleString;
		win->getCustomAttribute("WINDOW", &windowHandle);
		windowHandleString << windowHandle;
		parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
		_InputManager = OIS::InputManager::createInputSystem(parameters);
		_Keyboard = static_cast<OIS::Keyboard*>(_InputManager->createInputObject( OIS::OISKeyboard, false ));
		_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject( OIS::OISMouse, false ));

		_Cam = cam;
		_movementspeed = 50.0f;
		_sceneManager = sm;
		apertura = 0.0;
	}

	~AppFrameListener(){
		_InputManager->destroyInputObject(_Keyboard);
		_InputManager->destroyInputObject(_Mouse);
		OIS::InputManager::destroyInputSystem(_InputManager);
	}

	bool frameStarted(const Ogre::FrameEvent& evt){

		Ogre::Vector3 translate(0,0,0);
		
		for( int i = 0 ; i < 4 ; ++i ){
			torretas[i]->animState->addTime(evt.timeSinceLastFrame);
		}

		if ( nave->disparando ){
			for( int i = 0 ; i < 4 ; ++i ){

				nave->animState[i]->addTime(evt.timeSinceLastFrame);
			}
			
		}
	

		_Keyboard->capture();

		if(_Keyboard->isKeyDown(OIS::KC_ESCAPE)){
			return false;
		}


		if(_Keyboard->isKeyDown(OIS::KC_W)){
			
			bool seMovio = nave->moverAdelante();

			if ( !seMovio ){
				nave->moverAlInicio();
			}
		}

		/*if(_Keyboard->isKeyDown(OIS::KC_S)){
			translate += Ogre::Vector3(0,0,1);
		}*/

		if(_Keyboard->isKeyDown(OIS::KC_A)){
			nave->moverIzquierda();
		}

		if(_Keyboard->isKeyDown(OIS::KC_D)){
			nave->moverDerecha();
		}

		_Cam->moveRelative(translate*evt.timeSinceLastFrame * _movementspeed);
		
		if ( _Keyboard->isKeyDown(OIS::KC_E) ){
			nave->abrirAlas();
		}

		if ( _Keyboard->isKeyDown(OIS::KC_R) ){
			nave->cerrarAlas();
		}

		if ( _Keyboard->isKeyDown(OIS::KC_T) ){

				nave->disparar(nave->nodoNave, true);
			
		}

		/*_Mouse->capture();
		float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame* -1;
		float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
		_Cam->yaw(Ogre::Radian(rotX));
		_Cam->pitch(Ogre::Radian(rotY));*/

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

};

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
	camera = _sceneManager->createCamera("Camera");
	camera->setPosition(Ogre::Vector3(0.0,20.0,65.0));
	camera->lookAt(Ogre::Vector3(0.0,20.0,-1.0));
	camera->setNearClipDistance(5);

	Ogre::Viewport* viewport = window->addViewport(camera);
	viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
	camera->setAspectRatio(Ogre::Real(viewport->getActualWidth())/Ogre::Real(viewport->getActualHeight()));

}

// funcion donde se coloca lo que se desea desplegar.
void createScene(){

_sceneManager->setSkyDome(true, "MatFondo", 5, 8);
//_sceneManager->setSkyBox(true, "MatFondo", 100, false);

// Piso

Ogre::Entity* entEscena01 = _sceneManager->createEntity("proyectoStarW_mod_01.mesh");
entEscena01->setMaterialName("MatPiso");
_sceneManager->getRootSceneNode()->attachObject(entEscena01);

Ogre::Entity* entEscena02 = _sceneManager->createEntity("proyectoStarW_mod_02.mesh");
entEscena02->setMaterialName("MatPared2");
_sceneManager->getRootSceneNode()->attachObject(entEscena02);

Ogre::Entity* entEscena03 = _sceneManager->createEntity("proyectoStarW_mod_03.mesh");
entEscena03->setMaterialName("MatPared2");
_sceneManager->getRootSceneNode()->attachObject(entEscena03);

Ogre::Entity* entEscena04 = _sceneManager->createEntity("proyectoStarW_mod_04.mesh");
entEscena04->setMaterialName("MatPared");
_sceneManager->getRootSceneNode()->attachObject(entEscena04);

Ogre::Entity* entEscena05 = _sceneManager->createEntity("proyectoStarW_mod_05.mesh");
entEscena05->setMaterialName("MatPared");
_sceneManager->getRootSceneNode()->attachObject(entEscena05);

/*Ogre::Entity* entEscena06 = _sceneManager->createEntity("proyectoStarW_mod_06.mesh");
_sceneManager->getRootSceneNode()->attachObject(entEscena06);
*/

Ogre::Plane plane(Ogre::Vector3::UNIT_Y,-3);
Ogre::MeshManager::getSingleton().createPlane("final",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,plane,50,50,20,20,true,1,1,1,Ogre::Vector3::UNIT_Z);

Ogre::Entity* entPlano01 = _sceneManager->createEntity("PlanoFinal","final");
entPlano01->setMaterialName("MatFinal");
Ogre::SceneNode* nodoPlano = _sceneManager->createSceneNode("NodoPlanoFinal");
nodoPlano->attachObject(entPlano01);
nodoPlano->translate(0.0,10.0,-1300.0);
nodoPlano->rotate(Ogre::Vector3(1.0,0.0,0.0),Ogre::Radian(Ogre::Degree(-90.0)));
nodoPlano->rotate(Ogre::Vector3(0.0,0.0,1.0),Ogre::Radian(Ogre::Degree(180.0)));
_sceneManager->getRootSceneNode()->addChild(nodoPlano);

//Ogre::SceneNode* nodeFinal = _sceneManager->createSceneNode("nodeFinal");
//nodeFinal->attachObject(entEscena06);
//nodeFinal->scale(1.0,4.0,0.0);
//nodeFinal->translate(0.0,0.0,-1300.0);
//entEscena06->setMaterialName("MatFinal");

// Luz
_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

Ogre::SceneNode* nodeLuzPuntual01 = _sceneManager->createSceneNode("NodeLuzPuntual01");
_sceneManager->getRootSceneNode()->addChild(nodeLuzPuntual01);
nodeLuzPuntual01->setPosition(0,300,0);

Ogre::Light* LuzPuntual01 = _sceneManager->createLight("Luz01");
LuzPuntual01->setType(Ogre::Light::LT_POINT);
LuzPuntual01->setDiffuseColour(0.5,0.5,0.5);

nodeLuzPuntual01->attachObject(LuzPuntual01);

nave = new Nave(_sceneManager,camera);
_sceneManager->getRootSceneNode()->addChild(nave->nodoNave);


torretas[0] = new Torreta("torreta00",_sceneManager,-21.0,-8.0,-220.0,15.0);
torretas[1]= new Torreta("torreta01",_sceneManager,21.0,-8.0,-400.0,-15.0);
torretas[2] = new Torreta("torreta02",_sceneManager,-21.0,-8.0,-620.0,15.0);
torretas[3] = new Torreta("torreta03",_sceneManager,21.0,-8.0,-880.0,-15.0);

_sceneManager->getRootSceneNode()->addChild(torretas[0]->nodoTorreta);
_sceneManager->getRootSceneNode()->addChild(torretas[1]->nodoTorreta);
_sceneManager->getRootSceneNode()->addChild(torretas[2]->nodoTorreta);
_sceneManager->getRootSceneNode()->addChild(torretas[3]->nodoTorreta);

}

void renderOneFrame(){
Ogre::WindowEventUtilities::messagePump();
_keepRunning = _root->renderOneFrame();
}

bool keepRunning(){
return _keepRunning;
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
createCamera(window);
createScene();

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