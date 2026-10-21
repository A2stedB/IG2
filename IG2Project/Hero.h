#pragma once
#include "IG2Object.h"
#include "Character.h"

class Hero : public Character
{
public:
	Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh,float speed);

private:

};

