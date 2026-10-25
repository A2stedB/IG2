#pragma once
#include "IG2Object.h"
#include "Character.h"

class Hero : public Character
{
public:
	Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh,float speed);
	
	inline int getLives() const
	{
		return live;
	}

	inline int getPoints() const
	{
		return points;
	}

private:
	int live{ 0 };
	int points{ 0 };
};

