#include "Hero.h"
#include <OgreSceneManager.h>



Hero::Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh,float speed) :
	Character(initPos,node,sceneMng,mesh,speed)
{

	danceAnimationState = entity->getAnimationState("Dance");
	runBaseAnimationState = entity->getAnimationState("RunBase");
	runTopAnimationState = entity->getAnimationState("RunTop");

	danceAnimationState->setLoop(true);
	danceAnimationState->setEnabled(true);

	runBaseAnimationState->setLoop(true);
	runBaseAnimationState->setEnabled(false);

	runTopAnimationState->setLoop(true);
	runTopAnimationState->setEnabled(false);

	translateAnimation = NodeAnimation();

	translateAnimation.setup(mSM,"Translate animation",17);
	translateAnimation.attachTo(this->mNode);

	auto scale = []() {
		int sale = 10;
		return Vector3(sale, sale, sale);
		};

	// Como en blender, pero con las 3 propiedades
	translateAnimation.addKeyframe(0, Vector3(0, 0, 0), Quaternion(), scale());
	translateAnimation.addKeyframe(1, Vector3(10, 0, 0), Quaternion(Ogre::Degree(90), Vector3(0, 1, 0)), scale());
	translateAnimation.addKeyframe(6, Vector3(100, 0, 0), Quaternion(Ogre::Degree(90), Vector3(0, 1, 0)), scale());
	translateAnimation.addKeyframe(7, Vector3(100, 0, 0), Quaternion(Ogre::Degree(-90), Vector3(0, 1, 0)), scale());
	translateAnimation.addKeyframe(12, Vector3(-100, 0, 0), Quaternion(Ogre::Degree(-90), Vector3(0, 1, 0)),scale());
	translateAnimation.addKeyframe(13, Vector3(-100, 0, 0), Quaternion(Ogre::Degree(90),Vector3(0,1,0)),scale());
	translateAnimation.addKeyframe(16, Vector3(0, 0, 0), Quaternion(Ogre::Degree(90),Vector3(0,1,0)),scale());
	translateAnimation.addKeyframe(17, Vector3(0, 0, 0), Quaternion(),scale());

	translateAnimation.state->setEnabled(false);

	mNode->setInitialState();

	sword_l = sceneMng->createEntity("Sword.mesh");
	sword_r = sceneMng->createEntity("Sword.mesh");

	sword_l->setVisible(false);
	sword_r->setVisible(false);


	//danceAnimationState->setWeight(10);

	//ani_set = entity->getAllAnimationStates();
	//danceAnimationState = ani_set->createAnimationState("Bailar",0,4);
	//danceAnimationState->copyStateFrom(*(entity->getAnimationState("RunBase")));
	//sinbadSkeleton = entity->getSkeleton();

	//auto it = aux->getAnimationStateIterator().begin();
	//while (it != aux->getAnimationStateIterator().end()) {
	//	auto s = it->first;
	//	++it;
	//	cout << "Animation name: " << s << endl;
	//}
	//neckBone = sinbadSkeleton->getBone("Hand.L");

}

void Hero::loseLife()
{
	if (lives > 0)
		lives--;
}

void Hero::update(float dt)
{
	//ani_update(dt);
}

void Hero::ani_update(float dt)
{
	ani_elapsed_time += dt;

	std::cout << ani_elapsed_time << std::endl;

	if (is_dancing) 
	{
		std::cout << "Dancing" << std::endl;
		danceAnimationState->addTime(dt);

		if (ani_elapsed_time >= 4) 
		{
			is_dancing = false;
			danceAnimationState->setEnabled(false);
			danceAnimationState->setTimePosition(0);
			is_moving = true;

			runBaseAnimationState->setLoop(true);
			runBaseAnimationState->setEnabled(true);
			runTopAnimationState->setLoop(true);
			runTopAnimationState->setEnabled(true);

			translateAnimation.state->setEnabled(true);

			sword_l->setVisible(true);
			sword_r->setVisible(true);
		}
	}
	else if (is_moving) 
	{
		runBaseAnimationState->addTime(dt);
		runTopAnimationState->addTime(dt);
		translateAnimation.state->addTime(dt);

		std::cout << "Moving" << std::endl;

		if (ani_elapsed_time >= 10.5 && !has_sword)
		{
			this->entity->attachObjectToBone("Handle.R", sword_r);
			this->entity->attachObjectToBone("Handle.L", sword_l);
			has_sword = true;
		}
		if (ani_elapsed_time >= 21) 
		{
			is_moving = false;
			is_dancing = true;
			has_sword = false;
			ani_elapsed_time = 0;

			runBaseAnimationState->setEnabled(false);
			runBaseAnimationState->setTimePosition(0);

			runTopAnimationState->setEnabled(false);
			runTopAnimationState->setTimePosition(0);

			translateAnimation.state->setEnabled(false);
			translateAnimation.state->setTimePosition(0);

			this->entity->detachObjectFromBone(sword_l);
			this->entity->detachObjectFromBone(sword_r);
			sword_l->setVisible(false);
			sword_r->setVisible(false);

			danceAnimationState->setEnabled(true);
			std::cout << "Reseting" << std::endl;
		}
	}
}

void Hero::setup_ingame()
{
	danceAnimationState->setEnabled(false);
	danceAnimationState->setTimePosition(0);

	runBaseAnimationState->setEnabled(false);
	runBaseAnimationState->setTimePosition(0);

	runTopAnimationState->setEnabled(false);
	runTopAnimationState->setTimePosition(0);

	translateAnimation.state->setEnabled(false);
	translateAnimation.state->setTimePosition(0);

	entity->detachAllObjectsFromBone();
	sword_l->setVisible(false);
	sword_r->setVisible(false);
}
