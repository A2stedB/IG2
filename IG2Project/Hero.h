#pragma once
#include "IG2Object.h"
#include "Character.h"

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
private:
	int lives = 3;
	int points{ 0 };
};

