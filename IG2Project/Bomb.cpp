#include "Bomb.h"
#include <iostream>
#include <OgreParticleSystem.h>

Bomb::Bomb(Vector3 initPos, SceneNode* node, SceneManager* sceneMng ) :
	IG2Object(initPos,node,sceneMng)
{
	cuerpo = node->createChildSceneNode();
	mecha = node->createChildSceneNode();
	particleSystemNode = mecha->createChildSceneNode();

	Ogre::Entity* entity = sceneMng->createEntity("sphere.mesh");
	entity->setMaterialName("bombCuerpo");
	cuerpo->attachObject(entity);
	entity = sceneMng->createEntity("column.mesh");
	mecha->attachObject(entity);
	entity->setMaterialName("bombMecha");

	cuerpo->setScale(0.1,0.1,0.1);
	mecha->translate(Vector3(0,50,0));
	mecha->setScale(0.02, 0.02, 0.02);

	cuerpoAnimation = NodeAnimation();
	cuerpoAnimation.setup(sceneMng,"bomb_cuerpo",EXPLOSTION_TIME);
	cuerpoAnimation.attachTo(cuerpo);

	cuerpoAnimation.addKeyframe(0,cuerpo->getPosition(),cuerpo->getOrientation(),cuerpo->getScale());
	cuerpoAnimation.addKeyframe(1,cuerpo->getPosition(),cuerpo->getOrientation(), Vector3(MIN_SCALE, MIN_SCALE, MIN_SCALE));
	cuerpoAnimation.addKeyframe(2,cuerpo->getPosition(),cuerpo->getOrientation(), Vector3(MAX_SCALE, MAX_SCALE, MAX_SCALE));
	cuerpoAnimation.addKeyframe(3,cuerpo->getPosition(),cuerpo->getOrientation(), Vector3(MIN_SCALE, MIN_SCALE, MIN_SCALE));
	cuerpoAnimation.addKeyframe(EXPLOSTION_TIME, cuerpo->getPosition(), cuerpo->getOrientation(), Vector3(MAX_SCALE+0.1, MAX_SCALE+0.1, MAX_SCALE+0.1));

	//cuerpoAnimation.state->setTimePosition(0);

	//particleSystem = sceneMng->createParticleSystem("sistema_mecha","smokeParticle");
	particleSystem = sceneMng->createParticleSystem("sistema_mecha","animatedSmokeParticle");
	particleSystem->setEmitting(false);
	particleSystemNode->attachObject(particleSystem);
	particleSystemNode->translate(Vector3(0, 300, 0));

	mechaAnimation = NodeAnimation();
	mechaAnimation.setup(sceneMng, "particle_mecha", EXPLOSTION_TIME);
	mechaAnimation.attachTo(particleSystemNode);

	mechaAnimation.addKeyframe(0,particleSystemNode->getPosition(),particleSystemNode->getOrientation(),particleSystemNode->getScale());
	mechaAnimation.addKeyframe(EXPLOSTION_TIME,Vector3(0,0,0), particleSystemNode->getOrientation(), particleSystemNode->getScale());
	
	//toggle();
}

void Bomb::update(float dt)
{
	if (!is_active) return;

	elapsed_time += dt;
	cuerpoAnimation.addTime(dt);
	mechaAnimation.addTime(dt);
	std::cout << particleSystemNode->getPosition() << std::endl;

	if(elapsed_time >= explosion_time)
	{
		this->setVisible(false);
		cuerpoAnimation.state->setEnabled(false);
		mechaAnimation.state->setEnabled(false);
		particleSystem->setEmitting(false);
		is_active = false;
		std::cout << "Termino" << std::endl;
		exploded = true;
	}
}

void Bomb::toggle()
{
	if (is_active) return;
	else if (!is_active) 
	{
		std::cout << "Se activo una bomba" << std::endl;
		is_active = true;
		cuerpoAnimation.state->setEnabled(true);
		mechaAnimation.state->setEnabled(true);

		particleSystem->setEmitting(true);
	}
}

bool Bomb::isExploded() const {
	return exploded;
}

