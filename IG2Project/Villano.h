#pragma once
#include "Character.h"
class Villano : public Character
{
public:
	Villano(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, float speed);
	void update(float dt);
private:
	Ogre::SceneNode* cuerpoNode;
	Ogre::SceneNode* cabezaNode;
	Ogre::SceneNode* pecesNode;
	Ogre::SceneNode* pecesNode2;
};

