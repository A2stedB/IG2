#pragma once
#include "Character.h"
#include <OgreTimer.h>
#include "Animation.h"
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

	Ogre::Timer directionTimer;
	Ogre::Timer pecesNodeTimer;

	//Ogre::AnimationState* pecesNodeAnimationState;
	//Ogre::Animation* pecesNodeAnimation;
	//Ogre::NodeAnimationTrack* pecesNodeAnimationTrack;

	//std::vector<Ogre::TransformKeyFrame*> keyframes;

	NodeAnimation pecesNodeAnimation;
};

