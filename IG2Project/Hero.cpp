#include "Hero.h"

Hero::Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) :
	IG2Object(initPos,node,sceneMng,mesh)
{
	mNode->showBoundingBox(true);
}

Vector3 Hero::getCurrentDirection() const
{
	return current_direction;
}
