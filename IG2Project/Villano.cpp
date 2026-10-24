#include "Villano.h"
#include <random>
#include <OgreTimer.h>

#include <OgreAnimationTrack.h>
#include <OgreKeyFrame.h>

constexpr int UPDATE_TIME = 1000;

Villano::Villano(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, float speed) :
	Character(initPos, node, sceneMng, speed)
{
	cuerpoNode = this->createChildSceneNode();
	cabezaNode = this->createChildSceneNode();
	pecesNode = this->createChildSceneNode();

	Ogre::Entity* cuerpo = sceneMng->createEntity("penguin.mesh");
	cuerpoNode->attachObject(cuerpo);
	cuerpoNode->setPosition(Vector3::ZERO);

	Ogre::Entity* cabeza = sceneMng->createEntity("facial.mesh");
	cabezaNode->attachObject(cabeza);
	cabezaNode->setPosition(Vector3(0, 10, 0));

	//pecesNode->setPosition(initialPosition);
	pecesNode->scale(10, 10, 10);


	const int dx[] = { -1, 1, 0, 0 };
	const int dz[] = { 0, 0, -1, 1 };

	for (int i = 0; i < 4; ++i)
	{
		Ogre::SceneNode* nodo = pecesNode->createChildSceneNode();
		Ogre::Entity* fish = sceneMng->createEntity("fish.mesh");
		nodo->attachObject(fish);
		nodo->setPosition(Vector3(dx[i] * 7, 0, dz[i] * 7));
		nodo->yaw(Ogre::Radian(90 * dx[i]));
	}

	pecesNode->setInitialState();

	pecesNodeAnimation = mSM->createAnimation("peces_animation_1",4);
	pecesNodeAnimation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	pecesNodeAnimationTrack = pecesNodeAnimation->createNodeTrack(0);
	pecesNodeAnimationTrack->setAssociatedNode(pecesNode);


	
	keyframes.push_back(pecesNodeAnimationTrack->createNodeKeyFrame(0));
	keyframes.push_back(pecesNodeAnimationTrack->createNodeKeyFrame(1));
	keyframes.push_back(pecesNodeAnimationTrack->createNodeKeyFrame(2));
	keyframes.push_back(pecesNodeAnimationTrack->createNodeKeyFrame(3));
	keyframes.push_back(pecesNodeAnimationTrack->createNodeKeyFrame(4));

	keyframes[0]->setRotation(Quaternion());
	keyframes[1]->setRotation(Quaternion(Ogre::Radian(90),Vector3(0,1,0)));
	keyframes[2]->setRotation(Quaternion(Ogre::Radian(180),Vector3(0,1,0)));
	keyframes[3]->setRotation(Quaternion(Ogre::Radian(270),Vector3(0,1,0)));
	keyframes[4]->setRotation(Quaternion());

	pecesNodeAnimationState = mSM->createAnimationState("peces_animation_1");

	pecesNodeAnimationState->setLoop(true);
	pecesNodeAnimationState->setEnabled(true);


	//for (int i = 0;i < 8;++i) {
	//	Ogre::SceneNode* nodo = pecesNode->createChildSceneNode();
	//	Ogre::Entity* fish = sceneMng->createEntity("fish.mesh");
	//	nodo->attachObject(fish);
	//	nodo->setPosition(Vector3(0, 0, -5 + i * -5));
	//}


	this->setScale(Vector3(0.1f, 0.1f, 0.1f));

	directionTimer = Ogre::Timer();
	pecesNodeTimer = Ogre::Timer();
}

// update
void Villano::update(float dt) 
{
	//std::cout << getGridOrientation() << "\n";
	pecesNodeAnimationState->addTime(dt);
	if (directionTimer.getMilliseconds() < UPDATE_TIME) return;
	int rnd = rand() % 4;

	switch (rnd) {

		case(0): {
			current_direction = Vector3::NEGATIVE_UNIT_Z;
			break;
		}
		case(1): {
			current_direction = Vector3::UNIT_Z;
			break;
		}
		case(2): {
			current_direction = Vector3::NEGATIVE_UNIT_X;
			break;
		}
		case(3): {
			current_direction = Vector3::UNIT_X;
			break;
		}
	}

	directionTimer.reset();
}
 