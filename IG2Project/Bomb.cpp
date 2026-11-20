#include "Bomb.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <OgreParticleSystem.h>

Bomb::Bomb(Vector3 initPos, SceneNode* node, SceneManager* sceneMng ) :
	IG2Object(initPos,node,sceneMng)
{
	static int bombId = 0;
	std::string suffix = std::to_string(++bombId);

	cuerpo = node->createChildSceneNode();
	mecha = node->createChildSceneNode();
	particleSystemNode = mecha->createChildSceneNode();

	Ogre::Entity* entity = sceneMng->createEntity("sphere.mesh");
	entity->setMaterialName("bombCuerpo");
	cuerpo->attachObject(entity);
	entity = sceneMng->createEntity("column.mesh");
	mecha->attachObject(entity);
	entity->setMaterialName("bombMecha");

	cuerpo->setScale(0.07,0.07,0.07);
	mecha->translate(Vector3(0,10,0));
	mecha->setScale(0.02, 0.02, 0.02);

	cuerpoAnimation = NodeAnimation();
	cuerpoAnimation.setup(sceneMng, "bomb_cuerpo_" + suffix, EXPLOSTION_TIME);
	cuerpoAnimation.attachTo(cuerpo);

	cuerpoAnimation.addKeyframe(0,cuerpo->getPosition(),cuerpo->getOrientation(),cuerpo->getScale());
	cuerpoAnimation.addKeyframe(1,cuerpo->getPosition(),cuerpo->getOrientation(), Vector3(MIN_SCALE, MIN_SCALE, MIN_SCALE));
	cuerpoAnimation.addKeyframe(2,cuerpo->getPosition(),cuerpo->getOrientation(), Vector3(MAX_SCALE, MAX_SCALE, MAX_SCALE));
	cuerpoAnimation.addKeyframe(3,cuerpo->getPosition(),cuerpo->getOrientation(), Vector3(MIN_SCALE, MIN_SCALE, MIN_SCALE));
	cuerpoAnimation.addKeyframe(EXPLOSTION_TIME, cuerpo->getPosition(), cuerpo->getOrientation(), Vector3(MAX_SCALE + 0.01, MAX_SCALE + 0.01, MAX_SCALE + 0.01));

	//cuerpoAnimation.state->setTimePosition(0);
	cuerpoAnimation.state->setEnabled(false);

	//particleSystem = sceneMng->createParticleSystem("sistema_mecha","smokeParticle");
	particleSystem = sceneMng->createParticleSystem("sistema_mecha_" + suffix, "animatedSmokeParticle");
	particleSystem->setEmitting(false);
	particleSystemNode->attachObject(particleSystem);
	particleSystemNode->translate(Vector3(0, 300, 0));

	mechaAnimation = NodeAnimation();
	mechaAnimation.setup(sceneMng, "particle_mecha_" + suffix, EXPLOSTION_TIME);
	mechaAnimation.attachTo(particleSystemNode);

	mechaAnimation.addKeyframe(0,particleSystemNode->getPosition(),particleSystemNode->getOrientation(),particleSystemNode->getScale());
	mechaAnimation.addKeyframe(EXPLOSTION_TIME,Vector3(0,0,0), particleSystemNode->getOrientation(), particleSystemNode->getScale());
	//toggle();

	mechaAnimation.state->setEnabled(false);

	this->setVisible(false);
}

void Bomb::activate(const Vector3& position)
{
    if (is_active) return;

    setPosition(position);
    elapsed_time = 0.0f;
    exploded = false;
    is_active = true;

    cuerpoAnimation.state->setEnabled(false);
    mechaAnimation.state->setEnabled(false);
    cuerpoAnimation.state->setTimePosition(0);
    mechaAnimation.state->setTimePosition(0);
    cuerpoAnimation.state->setEnabled(true);
    mechaAnimation.state->setEnabled(true);

    particleSystem->clear();
    particleSystem->setEmitting(true);
    this->setVisible(true);
}

void Bomb::update(float dt)
{
    if (!is_active) return;

    elapsed_time += dt;
    cuerpoAnimation.state->addTime(dt);
    mechaAnimation.state->addTime(dt);

    if (elapsed_time >= EXPLOSTION_TIME)
    {
        // La logica de la explosion corresponde al Maze, NO a Bomb.
        is_active = false;
        exploded = true;
        particleSystem->setEmitting(false);
        particleSystem->clear();
        cuerpoAnimation.state->setEnabled(false);
        mechaAnimation.state->setEnabled(false);
        this->setVisible(false);
    }
}

void Bomb::toggle()
{
    if (!is_active) activate(getPosition());
}

bool Bomb::isExploded() const
{
    return exploded;
}

bool Bomb::isActive() const
{
    return is_active;
}
