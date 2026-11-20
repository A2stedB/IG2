#pragma once
#include "IG2Object.h"
#include "Character.h"
#include "Animation.h"
#include <OgreTimer.h>

class Hero : public Character
{
public:
	Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh,float speed);
	
	inline int getLives() const
	{
		return lives;
	}

	inline int getPoints() const
	{
		return points;
	}
	void loseLife();

	void update(float dt);

	void ani_update(float dt);

	void setup_ingame();

	bool canSpawnBomb();

	int getMaxBomb() const;
private:
	bool is_in_initial_screen = true;
	int lives = 3;
	int points{ 0 };

	Ogre::Entity* sword_l = nullptr;
	Ogre::Entity* sword_r = nullptr;


	AnimationState* danceAnimationState = nullptr;
	AnimationState* runBaseAnimationState = nullptr;
	AnimationState* runTopAnimationState = nullptr;
	NodeAnimation translateAnimation;

	bool is_dancing = true;
	bool is_moving = false;
	bool has_sword = false;

	float ani_elapsed_time = 0;
	float ani_total_duration = 21;

	Ogre::Timer bombTimer;

	float bomb_elapsedTime = 0;
	float bomb_coldown = 250; // milisegundos entre colocaciones

	int max_bomb = 2;

	
};

