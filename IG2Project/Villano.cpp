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

	// crear 3 cabezas

	SceneNode* cabezas[3];

	Vector3 posiciones[3] = {
		Vector3(0, 10, 0),    // Cabeza medio
		Vector3(-10, 10, 0),    // Cabeza izquierdo
		Vector3(10, 10, 0)      // Cabeza derecho
	};

	for (int i = 0; i < 3; ++i) {

		cabezas[i] = cabezaNode->createChildSceneNode();

		Ogre::Entity* cabeza = sceneMng->createEntity("facial.mesh");
		cabezas[i]->attachObject(cabeza);

		cabezas[i]->setPosition(posiciones[i]);
		cabezas[i]->setScale(Vector3(0.7f, 0.7f, 0.7f));

		cabezas[i]->setInitialState();
	}

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
	cabezaNodeAnimation = NodeAnimation();

	cabezaNodeAnimation.animation = mSM->createAnimation("cabezas_animation", 4);
	cabezaNodeAnimation.animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);

	for (int i = 0; i < 3; ++i) {

		Ogre::NodeAnimationTrack* track =
			cabezaNodeAnimation.animation->createNodeTrack(i);

		track->setAssociatedNode(cabezas[i]);

		track->createNodeKeyFrame(0)->setRotation(
			Quaternion(Ogre::Degree(0), Vector3::UNIT_Y));

		track->createNodeKeyFrame(1)->setRotation(
			Quaternion(Ogre::Degree(90), Vector3::UNIT_Y));

		track->createNodeKeyFrame(2)->setRotation(
			Quaternion(Ogre::Degree(180), Vector3::UNIT_Y));

		track->createNodeKeyFrame(3)->setRotation(
			Quaternion(Ogre::Degree(270), Vector3::UNIT_Y));

		track->createNodeKeyFrame(4)->setRotation(
			Quaternion(Ogre::Degree(360), Vector3::UNIT_Y));
	}

	cabezaNodeAnimation.state =
		mSM->createAnimationState("cabezas_animation");

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
 