#include "Villano.h"
#include <random>
Villano::Villano(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, float speed) :
	Character(initPos, node, sceneMng, speed)
{
	cuerpoNode = this->createChildSceneNode();
	cabezaNode = this->createChildSceneNode();
	pecesNode = this->createChildSceneNode();

	Ogre::Entity* cuerpo = sceneMng->createEntity("penguin.mesh");
	cuerpoNode->attachObject(cuerpo);
	cuerpoNode->setPosition(initialPosition);

	Ogre::Entity* cabeza = sceneMng->createEntity("facial.mesh");
	cabezaNode->attachObject(cabeza);
	cabezaNode->setPosition(initialPosition+Vector3(0,10,0));

	pecesNode->setPosition(initialPosition);
	pecesNode->scale(10, 10, 10);
	for (int i = 0;i < 8;++i) {
		Ogre::SceneNode* nodo = pecesNode->createChildSceneNode();
		Ogre::Entity* fish = sceneMng->createEntity("fish.mesh");
		nodo->attachObject(fish);
		nodo->setPosition(Vector3(0, 0, -5 + i * -5));
	}


}

// update
void Villano::update(float dt) {

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
}
 