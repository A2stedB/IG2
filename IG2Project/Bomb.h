#pragma once
#include "IG2Object.h"
#include "Animation.h"

constexpr float MAX_SCALE = 0.03F;
constexpr float MIN_SCALE = 0.01F;
constexpr float EXPLOSTION_TIME = 4.0F;

class Bomb : public IG2Object
{
public:
	Bomb(Vector3 initPos, SceneNode* node, SceneManager* sceneMng);

	void update(float dt);

	void toggle();

	bool isExploded() const;
private:
	Ogre::SceneNode* cuerpo = nullptr;
	Ogre::SceneNode* mecha = nullptr;
	Ogre::SceneNode* particleSystemNode = nullptr;
	Ogre::ParticleSystem* particleSystem = nullptr;
	
	bool is_active = true;
	bool exploded = false;
	float active_time = 0;
	float explosion_time = EXPLOSTION_TIME;
	float elapsed_time = 0.0f;

	NodeAnimation cuerpoAnimation;
	NodeAnimation mechaAnimation;
};

