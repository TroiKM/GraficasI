#include "Ogre.h"

class Torreta{

	public:

	float x,y,z,rotacion;
	Ogre::SceneManager* _sceneManager;

	Ogre::SceneNode* nodoTorreta;
	Ogre::AnimationState* animState;

	Torreta::Torreta( Ogre::String nombre , Ogre::SceneManager* sm , float x , float y , float z , float rotacion );

};