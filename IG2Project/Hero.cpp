#include "Hero.h"

float Hero::HERO_SPEED = 10.0f;

Hero::Hero(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh) :
	IG2Object(initPos,node,sceneMng,mesh)
{
	mNode->showBoundingBox(true);
}

Vector3 Hero::getCurrentDirection() const
{
	return current_direction;
}

void Hero::setDirection(Vector3 vector)
{
	current_direction = vector;
}


bool Hero::getDirectionModified() {

	return current_direction == getGridOrientation();
}
void Hero::rotateToDirection() {

	if (!getDirectionModified()) {
		Quaternion rotation = getGridOrientation().getRotationTo(current_direction);
		rotate(Quaternion(rotation));

	}
}

bool Hero::is180turn() {
	return current_direction == -getGridOrientation();
}
