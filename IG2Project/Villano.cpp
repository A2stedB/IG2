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

	pecesNodeAnimation = NodeAnimation();

	pecesNodeAnimation.animation= mSM->createAnimation("peces_animation_1",4);
	pecesNodeAnimation.animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	pecesNodeAnimation.track = pecesNodeAnimation.animation->createNodeTrack(0);
	pecesNodeAnimation.track->setAssociatedNode(pecesNode);


	
	pecesNodeAnimation.keyframes.push_back(pecesNodeAnimation.track->createNodeKeyFrame(0));
	pecesNodeAnimation.keyframes.push_back(pecesNodeAnimation.track->createNodeKeyFrame(1));
	pecesNodeAnimation.keyframes.push_back(pecesNodeAnimation.track->createNodeKeyFrame(2));
	pecesNodeAnimation.keyframes.push_back(pecesNodeAnimation.track->createNodeKeyFrame(3));
	pecesNodeAnimation.keyframes.push_back(pecesNodeAnimation.track->createNodeKeyFrame(4));

	auto& keyframes = pecesNodeAnimation.keyframes;
	keyframes[0]->setRotation(Quaternion(Ogre::Degree(90),Vector3(0,1,0)));
	keyframes[1]->setRotation(Quaternion(Ogre::Degree(90),Vector3(0,1,0)));
	keyframes[2]->setRotation(Quaternion(Ogre::Degree(180),Vector3(0,1,0)));
	keyframes[3]->setRotation(Quaternion(Ogre::Degree(270),Vector3(0,1,0)));
	keyframes[4]->setRotation(Quaternion(Ogre::Degree(90), Vector3(0, 1, 0)));

	pecesNodeAnimation.state = mSM->createAnimationState("peces_animation_1");

	pecesNodeAnimation.state->setLoop(true);
	pecesNodeAnimation.state->setEnabled(true);

	
	// crear animacion cabeza
	cabezaNode->setInitialState();

	cabezaNodeAnimation = NodeAnimation();

	cabezaNodeAnimation.animation = mSM->createAnimation("cabeza_animation_1", 4);
	cabezaNodeAnimation.animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);

	cabezaNodeAnimation.track = cabezaNodeAnimation.animation->createNodeTrack(0);
	cabezaNodeAnimation.track->setAssociatedNode(cabezaNode);

	cabezaNodeAnimation.keyframes.push_back(cabezaNodeAnimation.track->createNodeKeyFrame(0));
	cabezaNodeAnimation.keyframes.push_back(cabezaNodeAnimation.track->createNodeKeyFrame(1));
	cabezaNodeAnimation.keyframes.push_back(cabezaNodeAnimation.track->createNodeKeyFrame(2));
	cabezaNodeAnimation.keyframes.push_back(cabezaNodeAnimation.track->createNodeKeyFrame(3));
	cabezaNodeAnimation.keyframes.push_back(cabezaNodeAnimation.track->createNodeKeyFrame(4));

	auto& keyframesCabeza = cabezaNodeAnimation.keyframes;

	keyframesCabeza[0]->setRotation(Quaternion(Ogre::Degree(0), Vector3::UNIT_Y));
	keyframesCabeza[1]->setRotation(Quaternion(Ogre::Degree(-90), Vector3::UNIT_Y));
	keyframesCabeza[2]->setRotation(Quaternion(Ogre::Degree(-180), Vector3::UNIT_Y));
	keyframesCabeza[3]->setRotation(Quaternion(Ogre::Degree(-270), Vector3::UNIT_Y));
	keyframesCabeza[4]->setRotation(Quaternion(Ogre::Degree(-360), Vector3::UNIT_Y));

	cabezaNodeAnimation.state = mSM->createAnimationState("cabeza_animation_1");

	cabezaNodeAnimation.state->setLoop(true);
	cabezaNodeAnimation.state->setEnabled(true);

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
	pecesNodeAnimation.state->addTime(dt);
	cabezaNodeAnimation.state->addTime(dt);

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
 