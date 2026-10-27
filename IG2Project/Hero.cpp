#include "Hero.h"


Hero::Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh,float speed) :
	Character(initPos,node,sceneMng,mesh,speed)
{
}

void Hero::loseLife()
{
	if (lives > 0)
		lives--;
}